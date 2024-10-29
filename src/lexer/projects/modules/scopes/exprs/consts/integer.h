#pragma once

#include "const.h"

typedef struct {
    expr_const_base_t base;
    int value;
} expr_const_integer_t;