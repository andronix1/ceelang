#pragma once

#include "core/arr.h"
#include "core/str.h"

typedef struct {
    str_t type;
    str_t name;
} func_arg_t;

ARR_TYPED_ALIAS_DEFINE(func_args, func_arg_t);