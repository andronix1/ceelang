#include "ident.h"

raw_expr_read_result_t raw_expr_operator_reader(tokens_slice_t *tokens, message_base_t base, result_t *result) {
    READER_SETUP;
    EXPECT_TOKENS_COUNT_OR_NOT_THIS(1);
    token_t token = NEXT_TOKEN();
    binop_type_t binop_type = binop_variants_count;
    switch (token->kind) {
        case TOKEN_PLUS:
            binop_type = BINOP_PLUS;
            break;
        case TOKEN_MINUS:
            binop_type = BINOP_MINUS;
            break;
        case TOKEN_MULTIPLY:
            binop_type = BINOP_MULTIPLY;
            break;
        case TOKEN_DIVIDE:
            binop_type = BINOP_DIVIDE;
            break;
        case TOKEN_EQUALS:
            binop_type = BINOP_EQUALS;
            break;
        case TOKEN_NOT_EQUALS:
            binop_type = BINOP_NOT_EQUALS;
            break;
    }
    if (binop_type == binop_variants_count) {
        return NOT_THIS();
    }
    return (raw_expr_read_result_t)raw_expr_read_result_ok_new(
        read_result_base_new_simple(),
        (raw_expr_element_t)raw_expr_element_operator_new(
            raw_expr_element_base_new_simple(),
            binop_type
        ),
        1
    );
}