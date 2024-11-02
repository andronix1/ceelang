#pragma once

typedef enum {
    EXPR_IDENT,
    EXPR_CONST,
    EXPR_FUNCALL,
    EXPR_BINOP,
} expr_type_t;

typedef struct {
    expr_type_t type;
} expr_base_t;

typedef expr_base_t* expr_t;