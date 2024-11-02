#pragma once

#include "expr.h"

typedef enum {
    EXPR_BINOP_PLUS,
    EXPR_BINOP_MINUS,
    EXPR_BINOP_MULTIPLY,
    EXPR_BINOP_DIVIDE,
    EXPR_BINOP_EQUALS,
    EXPR_BINOP_NOT_EQUALS
} expr_binop_type_t;

typedef struct {
    expr_base_t base;
    expr_binop_type_t type;
    expr_t left, right;
} expr_binop_t;