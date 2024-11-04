#include "raw_expr.h"

SEALED_CHILD_IMPL(raw_expr_element, operator);
SEALED_CHILD_IMPL(raw_expr_element, ready);
SEALED_CHILD_IMPL_FREE(raw_expr_element, scope, {
    arr_free(&self->raw_expr);
});

SEALED_FREE_IMPL(raw_expr_element, {
    SEALED_SHOULD_BE_FREED(raw_expr_element, RAW_EXPR_SCOPE, scope)
}, 3);

void raw_expr_element_free_wrap(raw_expr_element_t* expr) {
    raw_expr_element_free(*expr);
}

ARR_TYPED_ALIAS_IMPL(raw_expr, raw_expr_element_t, raw_expr_element_free_wrap);