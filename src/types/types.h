#pragma once

#include "lexer/modules/scopes/scope.h"
#include "core/sealed.h"
#include "core/str.h"

SEALED_KIND(type,
	TYPE_SIMPLE
);

SEALED_BASE(type,
	(str_slice_t, name)
);

ARR_TYPED_ALIAS_DEFINE(types_scope, type_t);

bool types_scope_has_type(types_scope_slice_t *scope, str_slice_t *type); 

typedef struct {
       types_scope_t builtin;
} types_t;
