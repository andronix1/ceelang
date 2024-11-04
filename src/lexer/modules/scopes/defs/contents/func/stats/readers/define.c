#include "define.h"

stat_read_result_t stat_define_parse(tokens_slice_t *tokens, message_base_t base, result_t *result) {
    READER_SETUP;
    token_t define_token = NEXT_TOKEN();
    stat_define_type_t define_type;
    if (define_token->kind == TOKEN_CONST) {
        define_type = STAT_DEFINE_CONST;
    } else if (define_token->kind == TOKEN_VAR) {
        define_type = STAT_DEFINE_VAR;
    } else {
        return NOT_THIS();
    }
    EXPECT_MORE_TOKENS(4);
    token_t name_token = NEXT_TOKEN();
    EXPECT_TOKEN(name_token, TOKEN_IDENT);
    token_ident_t *name_ident_token = token_as_ident(name_token);

    EXPECT_TOKEN(NEXT_TOKEN(), TOKEN_COLON);

    token_t type_token = NEXT_TOKEN();
    EXPECT_TOKEN(type_token, TOKEN_IDENT);
    token_ident_t *type_ident_token = token_as_ident(type_token);

    expr_t expr = NULL;
    token_t next_token = NEXT_TOKEN();
    if (next_token->kind == TOKEN_SET) {
        tokens_slice_t assign_expr = subslice_after(tokens, len);
        assign_expr = tokens_before_circle_scoped(&assign_expr, TOKEN_SEMICOLON);
        len += assign_expr.len + 1;
        expr_parse(assign_expr, base, result, &expr);
    } else if (next_token->kind != TOKEN_SEMICOLON) {
        return INVALID(ERROR_INVALID_TOKEN);
    }

    return (stat_read_result_t)stat_read_result_ok_new(read_result_base_new_simple(),
        (stat_t)stat_define_new(stat_base_new_simple(),
            define_type,
            str_clone(&name_ident_token->ident),
            str_clone(&type_ident_token->ident),
            expr
        ),
        len
    );
}