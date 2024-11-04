#include "parse.h"

def_read_result_t fun_parse(tokens_slice_t *tokens, message_base_t base, result_t *result) {
    READER_SETUP;
    EXPECT_TOKENS_COUNT_OR_NOT_THIS(1);
    token_t def_token = NEXT_TOKEN();
    EXPECT_TOKEN_OR_NOT_THIS(def_token, TOKEN_FUN);

    // fun <IDENT> (
    EXPECT_TOKENS_COUNT(4);
    token_t fun_name_token = NEXT_TOKEN();
    EXPECT_TOKEN(fun_name_token, TOKEN_IDENT);
    EXPECT_TOKEN(NEXT_TOKEN(), TOKEN_OPENING_CIRCLE_BRACE);
    
    // <ARGS> )
    func_args_t args = func_args_new_with_cap(1);
    token_t token = NEXT_TOKEN();
    if (token->kind != TOKEN_CLOSING_CIRCLE_BRACE) {
        bool first_iter = true;
        do {
            token_t name_token = first_iter ? token : NEXT_TOKEN();
            first_iter = false;
            EXPECT_TOKEN(name_token, TOKEN_IDENT);
            EXPECT_TOKEN(NEXT_TOKEN(), TOKEN_COLON);
            token_t type_token = NEXT_TOKEN();
            EXPECT_TOKEN(type_token, TOKEN_IDENT);
            func_arg_t arg = {
                .name = str_clone(&token_as_ident(name_token)->ident),
                .type = str_clone(&token_as_ident(type_token)->ident)
            };
            arr_push(&args, &arg);
            token = NEXT_TOKEN();
            if (token->kind == TOKEN_CLOSING_CIRCLE_BRACE) {
                break;
            }
            EXPECT_TOKEN(token, TOKEN_COMMA);
        } while (token->kind == TOKEN_COMMA);
    }

    // : <TYPE> {
    str_t *return_type = NULL;
    EXPECT_TOKENS_COUNT(len + 1);
    token_t body_opening_token = NEXT_TOKEN();
    if (body_opening_token->kind == TOKEN_COLON) {
        EXPECT_TOKENS_COUNT(len + 2);
        token_t return_type_token = NEXT_TOKEN();
        EXPECT_TOKEN(return_type_token, TOKEN_IDENT);
        return_type = malloc(sizeof(str_t));
        *return_type = str_clone(&token_as_ident(return_type_token)->ident);
        len++;
    }
    tokens_slice_t scope_start = subslice_after(tokens, len - 1);
    tokens_slice_t scope;
    token_get_scope_err_t err = token_get_figure_scope(&scope_start, &scope);
    EXPECT_SCOPE_OR_NOT_THIS(err);

    len += scope.len + 1;

    // STATS PARSE
    stats_t stats = stats_new_with_cap(1);
    stats_parse(scope, base, result, &stats);

    str_t fun_name = token_as_ident(fun_name_token)->ident;

    def_t *def = malloc(sizeof(def_t));
    def->content = (def_content_t)def_content_func_new(def_content_base_new_simple(), return_type, args, stats);
    def->name = str_clone(&fun_name);

    return (def_read_result_t)def_read_result_ok_new(read_result_base_new_simple(), def, len);
}