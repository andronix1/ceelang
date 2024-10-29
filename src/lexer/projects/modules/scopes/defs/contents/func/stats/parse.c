#include "parse.h"

stat_parse_result_t stat_parse(tokens_slice_t tokens) {
    token_t first_token = tokens_try_get(&tokens, 0);
    if (first_token->type == TOKEN_CONST || first_token->type == TOKEN_VAR) {
        return define_stat_parse(tokens);
    } else if (first_token->type == TOKEN_IDENT) {
        token_t second_token = tokens_try_get(&tokens, 1);
        if (second_token->type != TOKEN_OPENING_CIRCLE_BRACE) {
            printf("ERROR: unknown token after ident(funcall expected): %s!\n", str_token[second_token->type]);
            exit(1);
        }
        return funcall_stat_parse(tokens);
    } else if (first_token->type == TOKEN_IF) {
        return if_stat_parse(tokens);
    } else if (first_token->type == TOKEN_RETURN) {
        return return_stat_parse(tokens);
    } else {
        printf("ERROR: unknown stat: ");
        for (size_t i = 0; i <= tokens.len; i++) {
            printf("%s; ", str_token[slice_at(token_t, &tokens, i)->type]);
        }
        printf("\n");
        exit(1);
    }
}

stats_t stats_parse(tokens_slice_t tokens) {
    size_t i = 0;
    token_t token;
    stats_t stats = arr_with_cap(stat_t, 1);
    while (i < tokens.len) {
        tokens_slice_t after = subslice_after(&tokens, i);
        stat_parse_result_t result = stat_parse(after);
        arr_push(stat_t, &stats, result.stat);
        i += result.len;
    }
    return stats;
}