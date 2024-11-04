#include "stat.h"

SEALED_CHILD_IMPL_FREE(stat, define, {
    str_free(&self->name);
    str_free(&self->type);
    if (self->expr) {
        expr_free(self->expr);
    }
});
SEALED_CHILD_IMPL_FREE(stat, funcall, {
    funcall_free(self->funcall);
    free(self->funcall);
});
SEALED_CHILD_IMPL_FREE(stat, return, {
    if (self->expr) {
        expr_free(self->expr);
    }
});
SEALED_CHILD_IMPL_FREE(stat, if, {
    if_cond_stat_free(&self->if_cond_stat);
    arr_free(&self->elifs);
    if (self->else_stats) {
        arr_free(self->else_stats);
        free(self->else_stats);
    }
});

SEALED_FREE_IMPL(stat, {
    SEALED_SHOULD_BE_FREED(stat, STAT_RETURN, return)
    SEALED_SHOULD_BE_FREED(stat, STAT_FUNCALL, funcall)
    SEALED_SHOULD_BE_FREED(stat, STAT_DEFINE, define)
    SEALED_SHOULD_BE_FREED(stat, STAT_IF, if)
}, 4);

void stat_free_wrap(stat_t *stat) {
    stat_free(*stat);
}

void if_cond_stat_free(if_cond_stat_t *stat) {
    expr_free(stat->cond);
    arr_free(&stat->stats);
}

ARR_TYPED_ALIAS_IMPL(stats, stat_t, stat_free_wrap);
ARR_TYPED_ALIAS_IMPL(elif_cond_stats, if_cond_stat_t, if_cond_stat_free);