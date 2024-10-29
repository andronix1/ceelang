#include "parse.h"

stat_t return_stat_parse(tokens_slice_t tokens) {
    assert(tokens_try_get(&tokens, 0)->type == TOKEN_RETURN);
    tokens = subslice_after(&tokens, 1);
    stat_return_t *result = malloc(sizeof(stat_return_t));
    result->base.type = STAT_RETURN;
    result->expr = NULL;
    if (tokens.len != 0) {
        result->expr = expr_parse(tokens);
    }
    return (stat_t)result;
}