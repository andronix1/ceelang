#include "if.h"

stat_read_result_t stat_if_parse(tokens_slice_t *tokens, message_base_t base, result_t *result) {
    READER_SETUP;
    token_t if_token = NEXT_TOKEN();
    EXPECT_TOKEN_OR_NOT_THIS(if_token, TOKEN_IF);
    if_cond_stat_t if_cond_stat;
    elif_cond_stats_t elif_stats = elif_cond_stats_new_with_cap(1);
    bool is_if = true;
    do {
        tokens_slice_t cond_tokens = subslice_after(tokens, len);
        cond_tokens = tokens_before_circle_scoped(&cond_tokens, TOKEN_OPENING_FIGURE_BRACE);
        expr_t cond_expr = NULL;
        expr_parse(cond_tokens, base, result, &cond_expr);
        len += cond_tokens.len;
        tokens_slice_t if_stat_tokens = subslice_after(tokens, len);
        token_get_scope_err_t err =  token_get_figure_scope(&if_stat_tokens, &if_stat_tokens);
        EXPECT_SCOPE(err);
        len += if_stat_tokens.len + 2;
        stats_t stats = stats_new_with_cap(1);
        stats_parse(if_stat_tokens, base, result, &stats);
        if_cond_stat_t cond_stat = {
            .cond = cond_expr,
            .stats = stats
        };
        if (is_if) {
            if_cond_stat = cond_stat;
            is_if = false;
        } else {
            arr_push(&elif_stats, &cond_stat);
        }
        if_token = NEXT_TOKEN();
    } while (if_token->kind == TOKEN_ELIF);
    stats_t *else_stats = NULL;
    if (if_token->kind == TOKEN_ELSE) {
        else_stats = malloc(sizeof(stats_t));
        *else_stats = stats_new_with_cap(1);
        tokens_slice_t else_tokens = subslice_after(tokens, len);
        token_get_scope_err_t err =  token_get_figure_scope(&else_tokens, &else_tokens);
        EXPECT_SCOPE(err);
        len += else_tokens.len + 2;
        stats_parse(else_tokens, base, result, else_stats);
    } else {
        len--;
    }
    return (stat_read_result_t)stat_read_result_ok_new(read_result_base_new_simple(), (stat_t)stat_if_new(stat_base_new_simple(), if_cond_stat, elif_stats, else_stats), len);
}