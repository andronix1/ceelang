#include "parse.h"

bool is_binop(token_t token) {
    return
        token->type == TOKEN_PLUS ||
        token->type == TOKEN_MINUS ||
        token->type == TOKEN_DIVIDE ||
        token->type == TOKEN_MULTIPLY;
}

typedef enum {
    RAW_EXPR_READY_EXPR,
    RAW_EXPR_OPERATOR,
    RAW_EXPR_SCOPE
} raw_expr_element_type_t;

typedef struct {
    raw_expr_element_type_t type;
} raw_expr_element_base_t;

typedef struct {
    raw_expr_element_base_t base;
    expr_binop_type_t type;
} raw_operator_expr_t;

typedef struct {
    raw_expr_element_base_t base;
    expr_t expr;
} raw_ready_expr_t;

typedef arr_t raw_expr_t;

typedef struct {
    raw_expr_element_base_t base;
    raw_expr_t expr;
} raw_scope_expr_t;

typedef raw_expr_element_base_t* raw_expr_element_t;

raw_expr_t raw_expr_parse(tokens_slice_t tokens) {
    raw_expr_t result = arr_with_cap(raw_expr_element_t, 1);
    for (size_t i = 0; i < tokens.len; i++) {
        token_t token = slice_at(token_t, &tokens, i);
        raw_expr_element_t element;
        if (
            token->type == TOKEN_PLUS ||
            token->type == TOKEN_MINUS ||
            token->type == TOKEN_MULTIPLY ||
            token->type == TOKEN_DIVIDE
        ) {
            raw_operator_expr_t *expr = malloc(sizeof(raw_operator_expr_t));
            expr->base.type = RAW_EXPR_OPERATOR;
            if (token->type == TOKEN_PLUS) {
                expr->type = EXPR_BINOP_PLUS;
            } else if (token->type == TOKEN_MINUS) {
                expr->type = EXPR_BINOP_MINUS;
            } else if (token->type == TOKEN_MULTIPLY) {
                expr->type = EXPR_BINOP_MULTIPLY;
            } else if (token->type == TOKEN_DIVIDE) {
                expr->type = EXPR_BINOP_DIVIDE;
            }
            element = (raw_expr_element_t)expr;
        } else if (token->type == TOKEN_OPENING_CIRCLE_BRACE) {
            tokens_slice_t scope_tokens = tokens_get_circle_scope(&tokens, i);
            i += scope_tokens.len + 1;
            raw_scope_expr_t *expr = malloc(sizeof(raw_scope_expr_t));
            expr->base.type = RAW_EXPR_SCOPE;
            expr->expr = raw_expr_parse(scope_tokens);
            element = (raw_expr_element_t)expr;
        } else if (token->type == TOKEN_IDENT) {
            token_ident_t *token_ident = (token_ident_t*)token;

            if (i + 1 < tokens.len && slice_at(token_t, &tokens, i + 1)->type == TOKEN_OPENING_CIRCLE_BRACE) {
                tokens_slice_t after = subslice_after(&tokens, i);
                tokens_slice_t funcall_tokens = tokens_before_circle_scoped_included(&after, TOKEN_CLOSING_CIRCLE_BRACE);
                i += funcall_tokens.len - 1;

                raw_ready_expr_t *raw_expr = malloc(sizeof(raw_ready_expr_t));
                expr_funcall_t *expr = malloc(sizeof(expr_funcall_t));

                expr->base.type = EXPR_FUNCALL;
                expr->funcall = funcall_parse(funcall_tokens);

                raw_expr->base.type = RAW_EXPR_READY_EXPR;
                raw_expr->expr = (expr_t)expr;
                element = (raw_expr_element_t)raw_expr;
            } else {
                raw_ready_expr_t *raw_expr = malloc(sizeof(raw_ready_expr_t));
                expr_ident_t *expr = malloc(sizeof(expr_ident_t));

                expr->base.type = EXPR_IDENT;
                expr->ident = token_ident->ident;

                raw_expr->base.type = RAW_EXPR_READY_EXPR;
                raw_expr->expr = (expr_t)expr;
                element = (raw_expr_element_t)raw_expr;
            }
        } else if (token->type == TOKEN_UINT) {
            raw_ready_expr_t *raw_expr = malloc(sizeof(raw_ready_expr_t));
            expr_const_integer_t *expr = malloc(sizeof(expr_const_integer_t));

            expr->base.base.type = EXPR_CONST;
            expr->base.type = EXPR_CONST_UINT;
            expr->value = ((token_uint_t*)token)->value;

            raw_expr->base.type = RAW_EXPR_READY_EXPR;
            raw_expr->expr = (expr_t)expr;
            element = (raw_expr_element_t)raw_expr;
        } else {
            printf("ERROR: unknown token in expr: %s\n", str_token[token->type]);
            exit(1);
        }
        arr_push(raw_expr_element_t, &result, element);
    }
    return result;
}

expr_t expr_parse_scope(raw_expr_t raw_expr);

expr_t raw_expr_to_expr(raw_expr_element_t element) {
    if (element->type == RAW_EXPR_READY_EXPR) {
        return ((raw_ready_expr_t*)element)->expr;
    } else if (element->type == RAW_EXPR_SCOPE) {
        return expr_parse_scope(((raw_scope_expr_t*)element)->expr);
    } else {
        printf("ERROR: invalid expr type. Must be ready or scope expr: %d!\n", element->type);
        exit(1);
    }
}

void raw_expr_collect_binops(raw_expr_t *raw_expr, bool(*filter)(expr_binop_type_t)) {
    for (size_t i = 1; i + 1 < raw_expr->slice.len; i++) {
        raw_expr_element_t element = arr_at(raw_expr_element_t, raw_expr, i);
        if (element->type != RAW_EXPR_OPERATOR) {
            continue;
        }
        raw_operator_expr_t *operator = (raw_operator_expr_t*)element;
        if (!filter(operator->type)) {
            continue;
        }
        raw_scope_expr_t *expr = malloc(sizeof(raw_scope_expr_t));
        raw_expr_element_t *prev_raw_expr = arr_ptr_at(raw_expr_element_t, raw_expr, i - 1);
        expr_t prev_expr = raw_expr_to_expr(*prev_raw_expr);
        expr_t next_expr = raw_expr_to_expr(arr_at(raw_expr_element_t, raw_expr, i + 1));
        raw_ready_expr_t *output_raw_expr = malloc(sizeof(raw_ready_expr_t));
        expr_binop_t *output_expr = malloc(sizeof(expr_binop_t));

        output_expr->base.type = EXPR_BINOP;
        output_expr->left = prev_expr;
        output_expr->right = next_expr;
        output_expr->type = operator->type;

        output_raw_expr->base.type = RAW_EXPR_READY_EXPR;
        output_raw_expr->expr = (expr_t)output_expr;

        *prev_raw_expr = (raw_expr_element_t)output_raw_expr;

        arr_remove(raw_expr, i);
        arr_remove(raw_expr, i);
        i--;
    }
}

bool binop_1(expr_binop_type_t type) {
    return type == EXPR_BINOP_DIVIDE || type == EXPR_BINOP_MULTIPLY;
}

bool binop_2(expr_binop_type_t type) {
    return type == EXPR_BINOP_MINUS || type == EXPR_BINOP_PLUS;
}

expr_t expr_parse_scope(raw_expr_t raw_expr) {
    if (raw_expr.slice.len == 0) {
        printf("ERROR: empty expr\n");
        exit(1);
    }

    raw_expr_collect_binops(&raw_expr, binop_1);
    raw_expr_collect_binops(&raw_expr, binop_2);

    if (raw_expr.slice.len != 1) {
        printf("ERROR: missing binop in expr(len = %d)!\n", raw_expr.slice.len);
        exit(1);
    }

    raw_expr_element_t expr = arr_at(raw_expr_element_t, &raw_expr, 0);
    switch (expr->type) {
        case RAW_EXPR_READY_EXPR:
            return ((raw_ready_expr_t*)expr)->expr;
        case RAW_EXPR_SCOPE:
            return expr_parse_scope(((raw_scope_expr_t*)expr)->expr);
        case RAW_EXPR_OPERATOR:
            printf("ERROR: only operator in expr\n", raw_expr.slice.len);
            exit(1);
    }
}

expr_t expr_parse(tokens_slice_t tokens) {
    raw_expr_t raw_expr = raw_expr_parse(tokens);
    expr_t result = expr_parse_scope(raw_expr);
    arr_free(&raw_expr);
    return result;
}