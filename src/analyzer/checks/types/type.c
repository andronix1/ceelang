#include "type.h"

ARR_TYPED_ALIAS_IMPL(types, type_t, NULL);

types_info_t types_info_new() {
	type_t t_i32 = type_base_new(TYPE_SIMPLE, str_slice_from_cstr("i32"));

	types_t builtin = types_new_with_cap(1);
	arr_push(&builtin, &t_i32);

	types_info_t result = {
		builtin = builtin
	};
	return result;
}

type_t type_resolve(types_info_t *types, str_slice_t *name) {
	SLICE_FOREACH(&types->builtin.slice, type_t, type, {
		if (str_slice_equals(&(*type)->name, name)) {
			return *type;
		}
	});
	return NULL;
}
