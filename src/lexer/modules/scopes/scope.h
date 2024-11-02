#pragma once

#include "defs/def.h"
#include "core/str.h"
#include "core/arr.h"

typedef struct {
	str_t name;
	defs_t defs;
} scope_t;

void scope_free(scope_t *scope);

ARR_TYPED_ALIAS_DEFINE(scopes, scope_t);