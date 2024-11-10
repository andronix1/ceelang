#pragma once

#include "core/sealed.h"
#include "core/str.h"

SEALED_KIND(type,
	TYPE_SIMPLE,
	TYPE_FUNC
);

SEALED_BASE(type);

SEALED_CHILD_DEFINE(type, TYPE_SIMPLE, simple,
	(str_slice_t, name)
);

ARR_TYPED_ALIAS_DEFINE(types, type_t);

SEALED_CHILD_DEFINE(type, TYPE_FUNC, func,
	(types_t, args),
	(type_t, returns)
);

typedef struct {
	types_t builtin;
} types_info_t;

types_info_t types_info_new();

extern type_t type_bool;
extern type_t type_i32;
extern type_t type_str;
extern type_t type_void;

type_t type_resolve_simple(types_info_t *types, str_slice_t *name);
