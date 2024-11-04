#include "ident.h"

void raw_expr_parse(tokens_slice_t slice, message_base_t base, result_t *result, raw_expr_t *raw_expr);

raw_expr_read_result_t raw_expr_scope_reader(tokens_slice_t *tokens, message_base_t base, result_t *result) {
    READER_SETUP;
    EXPECT_TOKENS_COUNT_OR_NOT_THIS(1);
    EXPECT_TOKEN_OR_NOT_THIS(NEXT_TOKEN(), TOKEN_OPENING_CIRCLE_BRACE);
    tokens_slice_t scope;
    token_get_scope_err_t err = token_get_circle_scope(tokens, &scope);
    EXPECT_SCOPE_OR_NOT_THIS(err);
    raw_expr_t expr = raw_expr_new_with_cap(1);
    raw_expr_parse(scope, base, result, &expr);
    return (raw_expr_read_result_t)raw_expr_read_result_ok_new(
        read_result_base_new_simple(),
        (raw_expr_element_t)raw_expr_element_scope_new(
            raw_expr_element_base_new_simple(),
            expr           
        ),
        scope.len + 2
    );
}