#pragma once

#include "../stat.h"
#include "lexer/projects/modules/scopes/exprs/funcall/funcall.h"

typedef struct {
    stat_base_t base;
    funcall_t funcall;
} stat_funcall_t;