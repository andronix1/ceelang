#include "parse.h"

typedef struct {
    def_t def;
    size_t len;
} def_parse_result_t;

def_parse_result_t def_parse(slice_t tokens) {
    if (tokens.len < 2) {
        printf("ERROR: invalid def tokens len < 2\n");
        exit(1);
    }
    token_t first_token = slice_at(token_t, &tokens, 0);
    if (first_token->type != TOKEN_FUN) {
        printf("ERROR: invalid first def token: %d\n", first_token->type);
        exit(1);
    }
    token_t second_token = slice_at(token_t, &tokens, 1);
    if (second_token->type != TOKEN_IDENT) {
        printf("ERROR: invalid def tokens: second token is not ident(%d)\n", second_token->type);
        exit(1);
    }
    token_ident_t ident_token = *(token_ident_t*)second_token;
    slice_t after_tokens = subslice_after(&tokens, 2);
    if (after_tokens.len == 0) {
        printf("ERROR: invalid def tokens: after_tokens.len == 0\n");
        exit(1);
    }
    switch (first_token->type) {
        case TOKEN_FUN:
            def_content_parse_result_t res = def_func_content_parse(after_tokens);
            def_parse_result_t result = {
                .def = {
                    .name = ident_token.ident,
                    .content = res.content
                },
                .len = res.len + 2
            };
            return result;
            break;
        default:
            printf("ERROR: invalid def token %d\n", first_token);
            exit(1);
            break;
    }
}

defs_t defs_parse(tokens_t tokens) {
    slice_t tokens_slice = tokens.slice;
    defs_t defs = arr_with_cap(def_t, 1);
    while (tokens_slice.len > 0) {
        def_parse_result_t result = def_parse(tokens_slice);
        arr_push(def_t, &defs, result.def);
        tokens_slice = subslice_after(&tokens_slice, result.len);
    }
    return defs;
}