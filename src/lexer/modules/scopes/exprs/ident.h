#pragma once

#include "core/str.h"
#include "expr.h"

typedef struct {
    expr_base_t base;
    str_t ident;
} expr_ident_t;