#include "type.h"

SEALED_CHILD_IMPL(type, simple);
SEALED_CHILD_IMPL(type, func);

ARR_TYPED_ALIAS_IMPL(types, type_t, NULL);
ARR_TYPED_ALIAS_IMPL(typed_func_args, typed_func_arg_t, NULL);

type_t type_bool;
type_t type_i32;
type_t type_str;
type_t type_void;

types_info_t types_info_new() {
	type_i32 = (type_t)type_simple_new(type_base_new_simple(), str_slice_from_cstr("i32"));
	type_void = (type_t)type_simple_new(type_base_new_simple(), str_slice_from_cstr("void"));
	type_str = (type_t)type_simple_new(type_base_new_simple(), str_slice_from_cstr("str"));
	type_bool = (type_t)type_simple_new(type_base_new_simple(), str_slice_from_cstr("bool"));

	types_t builtin = types_new_with_cap(1);
	arr_push(&builtin, &type_i32);
	arr_push(&builtin, &type_void);
	arr_push(&builtin, &type_str);
	arr_push(&builtin, &type_bool);

	types_info_t result = {
		builtin = builtin
	};
	return result;
}

type_t type_resolve_simple(types_info_t *types, str_slice_t *name) {
	SLICE_FOREACH(&types->builtin.slice, type_t, type, {
		if ((*type)->kind == TYPE_SIMPLE && str_slice_equals(&type_as_simple(*type)->name, name)) {
			return *type;
		}
	});
	return NULL;
}
