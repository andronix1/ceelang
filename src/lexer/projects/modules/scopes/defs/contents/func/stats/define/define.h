#pragma once

#include "core/str.h"
#include "lexer/projects/modules/scopes/exprs/expr.h"
#include "../stat.h"

typedef enum {
    STAT_DEFINE_CONST,
    STAT_DEFINE_VAR,
} stat_define_type_t;

typedef struct {
    stat_base_t base;
    stat_define_type_t define_type;
    str_t name;
    str_t type;
    expr_t expr; //OPTIONAL
} stat_define_t;