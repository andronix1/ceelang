#pragma once

#include "core/sealed.h"

SEALED_KIND(expr,
    EXPR_IDENT,
    EXPR_CONST,
    EXPR_FUNCALL,
    EXPR_BINOP
);

SEALED_BASE(expr);
SEALED_FREE_DEFINE(expr);