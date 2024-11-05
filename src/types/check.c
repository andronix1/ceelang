#include "check.h"

void types_check_func(def_content_func_t *def, message_base_t base, result_t *result, types_scope_t *scope_types) {
	PUSH_WARNING(WARNING_NOT_IMPLEMENTED);
}

void types_check_define(def_content_define_t *def, message_base_t base, result_t *result, types_scope_t *scope_types) {
	if (!types_scope_has_type(&scope_types->slice, &def->definition->type.slice)) {
		PUSH_ERROR(ERROR_UNKNOWN_TYPE);
	}
	// PUSH_WARNING(WARNING_NOT_IMPLEMENTED);
}

void types_check_def(def_t *def, message_base_t base, result_t *result, types_scope_t *scope_types) {
	SEALED_ASSERT_ALL_USED(def_content, 2);
	switch (def->content->kind) {
		case DEF_CONTENT_FUNC:
			types_check_func(def_content_as_func(def->content), base, result, scope_types);
			break;

		case DEF_CONTENT_DEFINE:
			types_check_define(def_content_as_define(def->content), base, result, scope_types);
			break;
	}
}

void types_check_scope(scope_t *scope, message_base_t base, result_t *result) {
	types_scope_t scope_types = types_scope_new_with_cap(1);
	type_t type_i32 = type_base_new(TYPE_SIMPLE, str_slice_from_cstr("i32"));
	arr_push(&scope_types, &type_i32);
	SLICE_FOREACH(&scope->defs.slice, def_t, def, {
		types_check_def(def, base, result, &scope_types);
	});
}
