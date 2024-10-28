#pragma once

#include "core/arr.h"
#include "core/str.h"

typedef struct {
    str_t type;
    str_t name;
} func_arg_t;

typedef arr_t func_args_t;