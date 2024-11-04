#pragma once

#include "core/str.h"
#include "core/sealed.h"

SEALED_KIND(expr,
    EXPR_IDENT,
    EXPR_CONST,
    EXPR_FUNCALL,
    EXPR_BINOP
);

SEALED_BASE(expr);
SEALED_FREE_DEFINE(expr);

// binop

typedef enum {
    BINOP_PLUS,
    BINOP_MINUS,
    BINOP_MULTIPLY,
    BINOP_DIVIDE,
    BINOP_EQUALS,
    BINOP_NOT_EQUALS,
    binop_variants_count
} binop_type_t;

SEALED_CHILD_DEFINE_FREE(expr, EXPR_BINOP, binop,
    (binop_type_t, type),
    (expr_t, left),
    (expr_t, right)
);

// ident

SEALED_CHILD_DEFINE_FREE(expr, EXPR_IDENT, ident,
    (str_t, ident)
);

// funcall

ARR_TYPED_ALIAS_DEFINE(funcall_args, expr_t);

typedef struct {
    str_t ident;
    funcall_args_t args;
} funcall_t;

void funcall_free(funcall_t *funcall);

SEALED_CHILD_DEFINE_FREE(expr, EXPR_FUNCALL, funcall,
    (funcall_t, funcall)
);

// const

SEALED_KIND(const,
    CONST_STR,
    CONST_INT
);

SEALED_BASE(const);
SEALED_FREE_DEFINE(const);

SEALED_CHILD_DEFINE_FREE(const, CONST_STR, str,
    (str_t, value)
);

SEALED_CHILD_DEFINE(const, CONST_INT, int,
    (uint64_t, value)
);

SEALED_CHILD_DEFINE_FREE(expr, EXPR_CONST, const,
    (const_t, value)
);