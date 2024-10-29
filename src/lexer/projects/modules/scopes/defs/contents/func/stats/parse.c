#include "parse.h"

stat_t stat_parse(tokens_slice_t tokens) {
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
    } else if (first_token->type == TOKEN_RETURN) {
        return return_stat_parse(tokens);
    } else {
        printf("ERROR: unknown stat start: %s!\n", str_token[first_token->type]);
        exit(1);
    }
}