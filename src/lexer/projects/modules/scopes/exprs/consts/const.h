#pragma once

#include "core/str.h"
#include "expr.h"

typedef enum {
    EXPR_CONST_INT
} expr_const_type_t;

typedef struct {
    expr_base_t base;
    expr_const_type_t type;
} expr_const_base_t;

typedef expr_const_base_t* expr_const_t;