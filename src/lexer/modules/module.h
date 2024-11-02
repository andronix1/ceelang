#pragma once

#include "scopes/scope.h"
#include "core/arr.h"

typedef struct {
	str_t name;
	scopes_t scopes;
} module_t;

ARR_TYPED_ALIAS_DEFINE(modules, module_t);