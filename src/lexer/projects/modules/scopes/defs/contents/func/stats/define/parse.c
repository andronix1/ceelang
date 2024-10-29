#include "parse.h"

stat_parse_result_t define_stat_parse(tokens_slice_t tokens) {
    assert(tokens.len > 2);
    tokens = tokens_before(&tokens, TOKEN_SEMICOLON);
    token_t declare_type_token = tokens_try_get(&tokens, 0);
    assert(declare_type_token->type == TOKEN_CONST || declare_type_token->type == TOKEN_VAR);
    stat_define_t *result = malloc(sizeof(stat_define_t));
    result->base.type = STAT_DEFINE;
    result->define_type = declare_type_token->type == TOKEN_CONST ? STAT_DEFINE_CONST : STAT_DEFINE_VAR;
    result->name = token_extract_ident(tokens_try_get(&tokens, 1));
    token_expect_type(tokens_try_get(&tokens, 2), TOKEN_COLON);
    result->type = token_extract_ident(tokens_try_get(&tokens, 3));
    result->expr = NULL;
    if (tokens.len > 4) {
        token_expect_type(tokens_try_get(&tokens, 4), TOKEN_SET);
        tokens_slice_t expr_tokens = subslice_after(&tokens, 5);
        result->expr = expr_parse(expr_tokens);
    }
    stat_parse_result_t data = {
        .len = tokens.len + 1,
        .stat = (stat_t)result
    };
    return data;
}