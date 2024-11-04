#include "stat.h"

SEALED_CHILD_IMPL(stat, define);
SEALED_CHILD_IMPL_FREE(stat, funcall, {
    funcall_free(&self->funcall);
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
}, 4);

void stat_free_wrap(stat_t *stat) {
    stat_free(*stat);
}

ARR_TYPED_ALIAS_IMPL(stats, stat_t, stat_free_wrap);