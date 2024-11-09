#pragma once

#include "core/str.h"
#include "../../../exprs/expr_base.h"

typedef enum {
    DEFINITION_CONST,
    DEFINITION_VAR,
} definition_type_t;

typedef struct {
    definition_type_t kind;
    str_t type;
    expr_t expr;
} definition_t;

void definition_free(definition_t *definition);
