#pragma once

#include "expr.h"
#include "core/sealed.h"

SEALED_KIND(raw_expr_element,
    RAW_EXPR_READY,
    RAW_EXPR_OPERATOR,
    RAW_EXPR_SCOPE
);

SEALED_BASE(raw_expr_element);
SEALED_FREE_DEFINE(raw_expr_element);

SEALED_CHILD_DEFINE(raw_expr_element, RAW_EXPR_OPERATOR, operator,
    (binop_type_t, type)
);

SEALED_CHILD_DEFINE(raw_expr_element, RAW_EXPR_READY, ready,
    (expr_t, expr)
);

ARR_TYPED_ALIAS_DEFINE(raw_expr, raw_expr_element_t);

SEALED_CHILD_DEFINE_FREE(raw_expr_element, RAW_EXPR_SCOPE, scope,
    (raw_expr_t, raw_expr)
);