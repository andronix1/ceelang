#include "funcall.h"

void funcall_free(funcall_t *funcall) {
    arr_free(&funcall->args);
    str_free(&funcall->ident);
}

void expr_free_wrap(expr_t *expr) {
    expr_free(*expr);
}

ARR_TYPED_ALIAS_IMPL(funcall_args, expr_t, expr_free_wrap);
