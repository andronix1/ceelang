#include "check.h"

#define TYPES_CHECK(type, name) if (!types_scope_has_type(&scope_types->slice, type)) PUSH_ERROR(error_unknown_type_new(error_base_new_simple(), name))

void types_check_definition(definition_t *definition, str_slice_t *name, message_base_t base, result_t *result, types_scope_t *scope_types) {
	TYPES_CHECK(&definition->type.slice, *name);
}

void types_check_stat(stat_t stat, message_base_t base, result_t *result, types_scope_t *scope_types) {
	SEALED_ASSERT_ALL_USED(stat, 4);
	switch (stat->kind) {
		case STAT_DEFINE: {
			stat_define_t *stat_define = stat_as_define(stat);
			types_check_definition(stat_define->definition, &stat_define->name.slice, base, result, scope_types);
			PUSH_WARNING(WARNING_NOT_IMPLEMENTED);
			break;
		}
		case STAT_RETURN:
			PUSH_WARNING(WARNING_NOT_IMPLEMENTED);
			break;
		case STAT_IF: // check stats
			PUSH_WARNING(WARNING_NOT_IMPLEMENTED);
			break;
		case STAT_FUNCALL:
			PUSH_WARNING(WARNING_NOT_IMPLEMENTED);
			break;
	}
}

void types_check_func(def_content_func_t *def, str_slice_t *name, message_base_t base, result_t *result, types_scope_t *scope_types) {
	if (def->return_type) {
		TYPES_CHECK(&def->return_type->slice, *name);
	}	
	SLICE_FOREACH(&def->args.slice, func_arg_t, arg, {	
		TYPES_CHECK(&arg->type.slice, arg->name.slice);	
	});
	SLICE_FOREACH(&def->stats.slice, stat_t, stat, {
		types_check_stat(*stat, base, result, scope_types);
	});
}

void types_check_define(def_content_define_t *def, str_slice_t *name, message_base_t base, result_t *result, types_scope_t *scope_types) {
	types_check_definition(def->definition, name, base, result, scope_types);
}

void types_check_def(def_t *def, message_base_t base, result_t *result, types_scope_t *scope_types) {
	SEALED_ASSERT_ALL_USED(def_content, 2);
	switch (def->content->kind) {
		case DEF_CONTENT_FUNC:
			types_check_func(def_content_as_func(def->content), &def->name.slice, base, result, scope_types);
			break;

		case DEF_CONTENT_DEFINE:
			types_check_define(def_content_as_define(def->content), &def->name.slice, base, result, scope_types);
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
