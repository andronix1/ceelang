#include "check.h"

ARR_TYPED_ALIAS_IMPL(symbols_stack, symbol_t, NULL);

void symbols_check_scope_rec(symbols_stack_t *stack, scope_t *scope, message_base_t base, result_t *result) {

}

symbol_t *symbols_stack_resolve(symbols_stack_t *stack, str_slice_t *name) {
	SLICE_FOREACH(&stack->slice, symbol_t, symbol, {
		if (str_slice_equals(&symbol->name, name)) {
			return symbol;
		}
	});
	return NULL;
}

void symbols_stack_push(symbols_stack_t *stack, symbol_t *symbol, message_base_t base, result_t *result) {
	if (symbols_stack_resolve(stack, &symbol->name)) {
		PUSH_ERROR(error_redefinition_new(error_base_new_simple(), symbol->name));
		return;
	}
	arr_push(stack, symbol);
}

#define SYMBOL_PUSH(symbol) symbols_stack_push(stack, symbol, base, result)

void symbols_stack_push_def(types_info_t *types, symbols_stack_t *stack, def_t *def, message_base_t base, result_t *result) {
	SEALED_ASSERT_ALL_USED(def_content, 2);
	symbol_t symbol = {
		.name = def->name.slice
	};
	switch (def->content->kind) {
		case DEF_CONTENT_FUNC: {
			def_content_func_t *def_func = def_content_as_func(def->content);
			if (!def_func->return_type) {
				return;
			}
			types_t func_args = types_new_with_cap(1);
			SLICE_FOREACH(&def_func->args.slice, func_arg_t, arg, {
				type_t arg_type = type_resolve_simple(types, &arg->type.slice);
				arr_push(&func_args, &arg_type);
			});
			symbol.type = (type_t)type_func_new(type_base_new_simple(), func_args, type_resolve_simple(types, &def_func->return_type->slice));
			break;
		}
		case DEF_CONTENT_DEFINE: {
			def_content_define_t *def_define = def_content_as_define(def->content);
			symbol.type = type_resolve_simple(types, &def_define->definition->type.slice);
			break;
		}
		default:
			return;
	}
	SYMBOL_PUSH(&symbol);
}

void symbols_check_scope(types_info_t *types, scope_t *scope, message_base_t base, result_t *result) {
	symbols_stack_t stack = symbols_stack_new_with_cap(1);
	
	SLICE_FOREACH(&scope->defs.slice, def_t, def, {
		symbols_stack_push_def(types, &stack, def, base, result);
	});

	SLICE_FOREACH(&stack.slice, symbol_t, symbol, {
		printf("symbol `");
		str_slice_dump(&symbol->name, stdout);
		printf("`\n");
	});

	arr_free(&stack);	
}
