#pragma once

#include "core/str.h"
#include "core/sealed.h"
#include "../funcall/funcall.h"
#include "expr_base.h"

// binop

typedef enum {
    BINOP_PLUS,
    BINOP_MINUS,
    BINOP_MULTIPLY,
    BINOP_DIVIDE,
    BINOP_EQUALS,
    BINOP_NOT_EQUALS,
    BINOP_LESS,
    BINOP_GREATER,
    BINOP_LE,
    BINOP_GE,
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

SEALED_CHILD_DEFINE_FREE(expr, EXPR_FUNCALL, funcall,
    (funcall_t*, funcall)
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