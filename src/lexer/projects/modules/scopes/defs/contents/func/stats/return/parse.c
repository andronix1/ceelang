#include "parse.h"

stat_parse_result_t return_stat_parse(tokens_slice_t tokens) {
    assert(tokens_try_get(&tokens, 0)->type == TOKEN_RETURN);
    tokens = subslice_after(&tokens, 1);
    tokens = tokens_before(&tokens, TOKEN_SEMICOLON);
    stat_return_t *result = malloc(sizeof(stat_return_t));
    result->base.type = STAT_RETURN;
    result->expr = NULL;
    if (tokens.len != 0) {
        result->expr = expr_parse(tokens);
    }
    stat_parse_result_t data = {
        .len = tokens.len + 2,
        .stat = (stat_t)result
    };
    return data;
}