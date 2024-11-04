#include "parse.h"

void expr_parse(tokens_slice_t slice, message_base_t base, result_t *result, expr_t *expr);

funcall_read_result_t funcall_reader(tokens_slice_t *tokens, message_base_t base, result_t *result) {
    READER_SETUP;
    EXPECT_TOKENS_COUNT_OR_NOT_THIS(2);
    token_t name_token = NEXT_TOKEN();
    EXPECT_TOKEN_OR_NOT_THIS(name_token, TOKEN_IDENT);
    tokens_slice_t scope = subslice_after(tokens, 1);
    token_get_scope_err_t err = token_get_circle_scope(&scope, &scope);
    EXPECT_SCOPE_OR_NOT_THIS(err);
    funcall_t *funcall = malloc(sizeof(funcall_t));
    funcall->args = funcall_args_new_with_cap(1);
    funcall->ident = str_clone(&token_as_str(name_token)->value);
    len += scope.len + 2;
    while (scope.len > 0) {
        tokens_slice_t arg_tokens = tokens_before_circle_scoped(&scope, TOKEN_COMMA);
        expr_t expr = NULL;
        expr_parse(arg_tokens, base, result, &expr);
        if (expr) {
            arr_push(&funcall->args, &expr);
        }
        scope = subslice_after(&scope, arg_tokens.len);
        if (scope.len > 0) {
            scope = subslice_after(&scope, 1);
        }
    }
    return (funcall_read_result_t)funcall_read_result_ok_new(
        read_result_base_new_simple(),
        funcall,
        len
    );
}