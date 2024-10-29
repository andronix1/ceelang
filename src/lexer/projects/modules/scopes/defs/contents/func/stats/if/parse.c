#include "parse.h"

stat_parse_result_t if_stat_parse(tokens_slice_t tokens) {
    stat_if_t *result = malloc(sizeof(stat_if_t));
    result->base.type = STAT_IF;
    tokens_slice_t cond_tokens = subslice_after(&tokens, 1);
    cond_tokens = tokens_before(&cond_tokens, TOKEN_OPENING_FIGURE_BRACE);
    tokens_slice_t if_tokens = tokens_get_figure_scope(&tokens, cond_tokens.len + 1);
    result->cond = expr_parse(cond_tokens);
    result->if_stats = stats_parse(if_tokens);
    stat_parse_result_t data = {
        .len = 1 + cond_tokens.len + if_tokens.len + 2,
        .stat = (stat_t)result
    };
    return data;
}