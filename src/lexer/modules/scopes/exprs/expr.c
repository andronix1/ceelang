#include "expr.h"

void funcall_free(funcall_t *funcall) {
    arr_free(&funcall->args);
    str_free(&funcall->ident);
}

SEALED_CHILD_IMPL_FREE(expr, binop, {
    expr_free(self->left);
    expr_free(self->right);
});
SEALED_CHILD_IMPL_FREE(expr, ident, {
    str_free(&self->ident);
});
SEALED_CHILD_IMPL_FREE(expr, funcall, {
    funcall_free(&self->funcall);
});
SEALED_CHILD_IMPL_FREE(expr, const, {
    const_free(self->value);
});
SEALED_FREE_IMPL(const, {
    SEALED_SHOULD_BE_FREED(const, CONST_STR, str)
}, 2);

SEALED_CHILD_IMPL_FREE(const, str, {
    str_free(&self->value);
});
SEALED_CHILD_IMPL(const, int);

SEALED_FREE_IMPL(expr, {
    SEALED_SHOULD_BE_FREED(expr, EXPR_BINOP, binop)
    SEALED_SHOULD_BE_FREED(expr, EXPR_IDENT, ident)
    SEALED_SHOULD_BE_FREED(expr, EXPR_FUNCALL, funcall)
    SEALED_SHOULD_BE_FREED(expr, EXPR_CONST, const)
}, 4);

void expr_free_wrap(expr_t *expr) {
    expr_free(*expr);
}

ARR_TYPED_ALIAS_IMPL(funcall_args, expr_t, expr_free_wrap);
