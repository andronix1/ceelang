#include "parse.h"

stat_parse_result_t funcall_stat_parse(tokens_slice_t tokens) {
    tokens = tokens_before(&tokens, TOKEN_SEMICOLON);
    stat_funcall_t *result = malloc(sizeof(stat_funcall_t));
    result->base.type = STAT_FUNCALL;
    result->funcall = funcall_parse(tokens);
    stat_parse_result_t data = {
        .len = tokens.len + 1,
        .stat = (stat_t)result
    };
    return data;
}