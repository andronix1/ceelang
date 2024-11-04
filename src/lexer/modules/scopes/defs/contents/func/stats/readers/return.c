#include "return.h"

stat_read_result_t stat_return_parse(tokens_slice_t *tokens, message_base_t base, result_t *result) {
    READER_SETUP;
    EXPECT_TOKEN_OR_NOT_THIS(NEXT_TOKEN(), TOKEN_RETURN);
    expr_t expr = NULL;
    tokens_slice_t expr_tokens = tokens_before(tokens, TOKEN_SEMICOLON);
    expr_tokens = subslice_after(&expr_tokens, 1);
    expr_parse(expr_tokens, base, result, &expr);
    len += expr_tokens.len + 1;
    return (stat_read_result_t)stat_read_result_ok_new(read_result_base_new_simple(), (stat_t)stat_return_new(stat_base_new_simple(), expr), len);
}