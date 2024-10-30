#include "parse.h"

stat_parse_result_t if_stat_parse(tokens_slice_t tokens) {
    stat_if_t *result = malloc(sizeof(stat_if_t));
    result->base.type = STAT_IF;
    size_t len = 1;
    tokens_slice_t cond_tokens = subslice_after(&tokens, 1);
    cond_tokens = tokens_before(&cond_tokens, TOKEN_OPENING_FIGURE_BRACE);
    len += cond_tokens.len;
    tokens_slice_t if_tokens = tokens_get_figure_scope(&tokens, cond_tokens.len + 1);
    len += if_tokens.len + 2;
    result->cond = expr_parse(cond_tokens);
    result->if_stats = stats_parse(if_tokens);
    if (len < tokens.len && slice_at(token_t, &tokens, len)->type == TOKEN_ELSE) {
        len += 1;
        tokens_slice_t else_tokens = tokens_get_figure_scope(&tokens, len);
        len += else_tokens.len + 2;
        result->else_stats = malloc(sizeof(stats_t));
        *result->else_stats = stats_parse(else_tokens);
    }
    stat_parse_result_t data = {
        .len = len,
        .stat = (stat_t)result
    };
    return data;
}