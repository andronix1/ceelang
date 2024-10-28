#pragma once

#include "scopes/scope.h"
#include "core/arr.h"

typedef struct {
	str_t name;
	scopes_t scopes;
} module_t;

typedef arr_t modules_t;
