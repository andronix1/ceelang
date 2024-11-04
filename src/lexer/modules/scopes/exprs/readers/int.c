#include "ident.h"

raw_expr_read_result_t raw_expr_int_reader(tokens_slice_t *tokens, message_base_t base, result_t *result) {
    READER_SETUP;
    EXPECT_TOKENS_COUNT_OR_NOT_THIS(1);
    token_t token = NEXT_TOKEN();
    EXPECT_TOKEN_OR_NOT_THIS(token, TOKEN_UINT);
    uint64_t value = token_as_uint(token)->value;
    return (raw_expr_read_result_t)raw_expr_read_result_ok_new(
        read_result_base_new_simple(),
        (raw_expr_element_t)raw_expr_element_ready_new(
            raw_expr_element_base_new_simple(),
            (expr_t)expr_const_new(
                expr_base_new_simple(),
                (const_t)const_int_new(
                    const_base_new_simple(),
                    value
                )
            )
        ),
        1
    );
}