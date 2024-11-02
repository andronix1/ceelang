#include "parse.h"

funcall_t funcall_parse(tokens_slice_t tokens) {
    token_t ident_token = tokens_try_get(&tokens, 0);
    str_t ident = token_extract_ident(ident_token);

    tokens_slice_t scope = tokens_get_circle_scope(&tokens, 1);
    funcall_args_t args = arr_with_cap(funcall_arg_t, 1);
    while (scope.len > 0) {
        tokens_slice_t arg_tokens = tokens_before_circle_scoped(&scope, TOKEN_COMMA);
        arr_push(funcall_arg_t, &args, expr_parse(arg_tokens));
        if (arg_tokens.len == scope.len) {
            break;
        }
        scope = subslice_after(&scope, arg_tokens.len + 1);
    }
    funcall_t funcall = {
        .ident = ident,
        .args = args
    };
    return funcall;
}