#include "parse.h"

def_content_parse_result_t def_func_content_parse(tokens_slice_t tokens) {
    if (tokens_try_get(&tokens, 0)->type != TOKEN_OPENING_CIRCLE_BRACE) {
        printf("%d\n", tokens_try_get(&tokens, 0)->type);
        printf("ERROR: expected fun args start\n");
        exit(1);
    }
    arr_t args = arr_with_cap(func_arg_t, 1);
    size_t i = 1;
    while (i < tokens.len) {
        token_t token = tokens_try_get(&tokens, i++);
        if (token->type == TOKEN_CLOSING_CIRCLE_BRACE) {
            break;
        }
        token_t a = tokens_try_get(&tokens, i + 2);
        if (i + 2 >= tokens.len) {
            printf("ERROR: too few tokens for arg\n");
            exit(1);
        }
        if (token->type == TOKEN_COMMA) {
            if (args.slice.len == 0) {
                printf("ERROR: starting fun in comma args\n");
                exit(1);
            }
            token = tokens_try_get(&tokens, i++);
        }
        token_t colon_token = tokens_try_get(&tokens, i++);
        token_t type_token = tokens_try_get(&tokens, i++);
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
    str_t *return_type = NULL;
    if (tokens_try_get(&tokens, i)->type == TOKEN_COLON) {
        token_t type_token = tokens_try_get(&tokens, ++i);
        return_type = malloc(sizeof(str_t));
        str_t ident = token_extract_ident(type_token);
        memcpy(return_type, &ident, sizeof(str_t));
        i++;
    }
    tokens_slice_t scope_tokens = tokens_get_figure_scope(&tokens, i);
    stats_t stats = stats_parse(scope_tokens);
    i += scope_tokens.len + 1;
    def_content_func_t *content = malloc(sizeof(def_content_func_t));
    content->type = DEF_CONTENT_FUNC;
    content->args = args;
    content->stats = stats;
    content->return_type = return_type;
    def_content_parse_result_t result = {
        .content = (def_content_t)content,
        .len = i + 1
    };
    return result;
}