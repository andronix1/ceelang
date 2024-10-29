#include "parse.h"

stat_t stat_parse(slice_t tokens) {
    token_t first_token = tokens_try_get(&tokens, 0);
    if (first_token->type == TOKEN_CONST || first_token->type == TOKEN_VAR) {
        return define_stat_parse(tokens);
    } else {
        printf("ERROR: unknown stat start: %s!\n", str_token[first_token->type]);
        exit(1);
    }
}