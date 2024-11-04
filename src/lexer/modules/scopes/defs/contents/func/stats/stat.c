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
SEALED_CHILD_IMPL(stat, if);

SEALED_FREE_IMPL(stat, {
    SEALED_SHOULD_BE_FREED(stat, STAT_RETURN, return)
    SEALED_SHOULD_BE_FREED(stat, STAT_FUNCALL, funcall)
    SEALED_SHOULD_BE_FREED(stat, STAT_DEFINE, define)
}, 4);

void stat_free_wrap(stat_t *stat) {
    stat_free(*stat);
}

ARR_TYPED_ALIAS_IMPL(stats, stat_t, stat_free_wrap);