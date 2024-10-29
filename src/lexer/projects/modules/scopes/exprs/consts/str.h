#pragma once

#include "const.h"

typedef struct {
    expr_const_base_t base;
    str_t value;
} expr_const_str_t;