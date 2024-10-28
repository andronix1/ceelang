#include "parse.h"

def_content_parse_result_t def_func_content_parse(slice_t tokens) {
    if (tokens.len < 3) {
        printf("ERROR: fun input tokens length < 3\n");
        exit(1);
    }
    if (slice_at(token_t, &tokens, 0)->type != TOKEN_OPENING_CIRCLE_BRACE) {
        printf("%d\n", slice_at(token_t, &tokens, 0)->type);
        printf("ERROR: expected fun args start\n");
        exit(1);
    }
    arr_t args = arr_with_cap(func_arg_t, 1);
    size_t i = 1;
    while (i < tokens.len) {
        token_t token = slice_at(token_t, &tokens, i++);
        if (token->type == TOKEN_CLOSING_CIRCLE_BRACE) {
            break;
        }
        token_t a = slice_at(token_t, &tokens, i + 2);
        if (i + 2 >= tokens.len) {
            printf("ERROR: too few tokens for arg\n");
            exit(1);
        }
        if (token->type == TOKEN_COMMA) {
            if (args.slice.len == 0) {
                printf("ERROR: starting fun in comma args\n");
                exit(1);
            }
            token = slice_at(token_t, &tokens, i++);
        }
        token_t colon_token = slice_at(token_t, &tokens, i++);
        token_t type_token = slice_at(token_t, &tokens, i++);
        if (token->type != TOKEN_IDENT) {
            printf("ERROR: name token is not ident\n");
            exit(1);
        }
        if (colon_token->type != TOKEN_COLON) {
            printf("ERROR: second token is not colon\n");
            exit(1);
        }
        if (type_token->type != TOKEN_IDENT) {
            printf("ERROR: type token is not ident\n");
            exit(1);
        }
        func_arg_t arg = {
            .name = ((token_ident_t*)token)->ident,
            .type = ((token_ident_t*)type_token)->ident,
        };
        arr_push(func_arg_t, &args, arg);
    }
    if (i >= tokens.len) {
        printf("ERROR: fun unexpected end\n");
        exit(1);
    }
    // if (slice_at(token_t, &tokens, 1)->type != TOKEN_CLOSING_CIRCLE_BRACE) {
    //     printf("ERROR: fun args not implemented\n");
    //     exit(1);
    // }
    if (slice_at(token_t, &tokens, i)->type != TOKEN_OPENING_FIGURE_BRACE) {
        printf("ERROR: fun body start not started\n");
        exit(1);
    }
    i++;
    size_t figure_brace_len = 1;
    while (i < tokens.len) {
        token_t token = slice_at(token_t, &tokens, i);
        switch (token->type) {
            case TOKEN_OPENING_FIGURE_BRACE:
                figure_brace_len++;
                break;
            case TOKEN_CLOSING_FIGURE_BRACE:
                figure_brace_len--;
                break;
            default:
                printf("WARNING: ignoring token %d\n", token->type);
                break;
        }
        if (figure_brace_len == 0) {
            break;
        }
        i++;
    }
    if (figure_brace_len != 0) {
        printf("ERROR: looks like you didn't closed or didn't opened figure brace\n");
        exit(1);
    }
    def_content_func_t *content = malloc(sizeof(def_content_func_t));
    content->type = DEF_CONTENT_FUNC;
    content->args = args;
    def_content_parse_result_t result = {
        .content = (def_content_t)content,
        .len = i + 1
    };
    return result;
}