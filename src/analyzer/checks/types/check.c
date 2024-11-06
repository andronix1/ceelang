#include "check.h"

#define TYPE_NAME_CHECK(typename, of) if (!type_resolve(types, typename)) PUSH_ERROR(error_unknown_type_new(error_base_new_simple(), of))

void types_check_definition(types_info_t *types, definition_t *definition, str_slice_t *name, message_base_t base, result_t *result) {
	TYPE_NAME_CHECK(&definition->type.slice, *name);
}

void types_check_stat(types_info_t *types, stat_t stat, message_base_t base, result_t *result) {
	SEALED_ASSERT_ALL_USED(stat, 4);
	switch (stat->kind) {
		case STAT_DEFINE: {
			stat_define_t *stat_define = stat_as_define(stat);
			types_check_definition(types, stat_define->definition, &stat_define->name.slice, base, result);
			break;
		}
		case STAT_IF: {
			
			break;
		}
	}	
}

void types_check_def_content_func(types_info_t *types, def_content_func_t *content, str_slice_t *name, message_base_t base, result_t *result) {
	if (content->return_type) {
		TYPE_NAME_CHECK(&content->return_type->slice, *name);
	}
	SLICE_FOREACH(&content->args.slice, func_arg_t, arg, {
		TYPE_NAME_CHECK(&arg->type.slice, arg->name.slice);
	});
	SLICE_FOREACH(&content->stats.slice, stat_t, stat, {
		types_check_stat(types, *stat, base, result);
	});
}

void types_check_def_content_define(types_info_t *types, def_content_define_t *content, str_slice_t *name, message_base_t base, result_t *result) {
	types_check_definition(types, content->definition, name, base, result);
}

void types_check_def(types_info_t *types, def_t *def, message_base_t base, result_t *result) {
	SEALED_ASSERT_ALL_USED(def_content, 2);
	switch (def->content->kind) {
		case DEF_CONTENT_FUNC:
			types_check_def_content_func(types, def_content_as_func(def->content), &def->name.slice, base, result);
			break;
		case DEF_CONTENT_DEFINE:
			types_check_def_content_define(types, def_content_as_define(def->content), &def->name.slice, base, result);
			break;
	}
}

void types_check_scope(types_info_t *types, scope_t *scope, message_base_t base, result_t *result) {
	SLICE_FOREACH(&scope->defs.slice, def_t, def, {
		types_check_def(types, def, base, result);		
	});	
}
