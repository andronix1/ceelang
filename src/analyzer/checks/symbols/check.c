#include "check.h"

ARR_TYPED_ALIAS_IMPL(symbols_stack, symbol_t, NULL);

str_t type_to_str(type_t type) {

}

void symbol_expected_type(str_slice_t of, type_t expected, type_t found, message_base_t base, result_t *result) {
	if (expected != found) PUSH_ERROR(error_invalid_type_new(error_base_new_simple(), of, type_to_str(expected), type_to_str(found)));
}

#define SYMBOL_EXPECTED_TYPE(of, expected, found) symbol_expected_type(of, expected, found, base, result)

type_t symbols_check_expr(types_info_t *types, str_slice_t *name, expr_t expr, message_base_t base, result_t *result) {
	SEALED_ASSERT_ALL_USED(expr, 4);
	switch (expr->kind) {
		case EXPR_IDENT: {
			break;
		}
		case EXPR_FUNCALL: {
			PUSH_WARNING(WARNING_NOT_IMPLEMENTED);
			return NULL;
		}
		case EXPR_CONST: {
			expr_const_t *expr_const = expr_as_const(expr);
			SEALED_ASSERT_ALL_USED(const, 2);
			const_t value = expr_const->value;
			char *name;
			switch (value->kind) {
				case CONST_INT: {
					name = "i32";
					break;
				}
				case CONST_STR: {
					name = "str";
					break;
				}
			}
			str_slice_t name_slice = str_slice_from_cstr(name);
			return type_resolve_simple(types, &name_slice);
			break;
		}
		case EXPR_BINOP: {
			expr_binop_t *expr_binop = expr_as_binop(expr);
			type_t ltype = symbols_check_expr(types, name, expr_binop->left, base, result);
			type_t rtype = symbols_check_expr(types, name, expr_binop->right, base, result);
			if (ltype != rtype) {
				SYMBOL_EXPECTED_TYPE(*name, ltype, rtype);
				return NULL;
			}
			return ltype;
		}
	}
	// TODO: check with ignore name
}

void symbols_check_definition(types_info_t *types, str_slice_t *name, symbols_stack_t *stack, definition_t *definition, message_base_t base, result_t *result) {
	if (definition->expr) {
		type_t expr_type = symbols_check_expr(types, name, definition->expr, base, result);
		SYMBOL_EXPECTED_TYPE(*name, type_resolve_simple(types, &definition->type.slice), expr_type);
	}
}

void symbols_check_def(types_info_t *types, symbols_stack_t *stack, def_content_t content, message_base_t base, result_t *result) {
	SEALED_ASSERT_ALL_USED(def_content, 2);
	switch (content->kind) {
		case DEF_CONTENT_FUNC: {
			def_content_func_t *def_func = def_content_as_func(content);
			break;
		}
		case DEF_CONTENT_DEFINE: {
			def_content_define_t *def_define = def_content_as_define(content);
			break;
		}
	}	
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

	SLICE_FOREACH(&scope->defs.slice, def_t, def, {
		symbols_check_def(types, &stack, def->content, base, result);
	});

	SLICE_FOREACH(&stack.slice, symbol_t, symbol, {
		printf("symbol `");
		str_slice_dump(&symbol->name, stdout);
		printf("`\n");
	});

	arr_free(&stack);	
}
