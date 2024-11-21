#include "check.h"

ARR_TYPED_ALIAS_IMPL(symbols_stack, symbol_t, NULL);

str_t type_to_str(type_t type) {
	if (!type) {
		str_slice_t s = str_slice_from_cstr("???");
		return str_copy_from_slice(&s);
	}
	SEALED_ASSERT_ALL_USED(type, 2);
	switch (type->kind) {
		case TYPE_SIMPLE: {
			return str_copy_from_slice(&type_as_simple(type)->name);
		}
		case TYPE_FUNC: {
			str_slice_t s = str_slice_from_cstr("fun");
			return str_copy_from_slice(&s);
		}
	}
}

void symbol_expected_type(str_slice_t of, type_t expected, type_t found, message_base_t base, result_t *result) {
	if (expected != NULL && found != NULL && expected != found) PUSH_ERROR(error_invalid_type_new(error_base_new_simple(), of, type_to_str(expected), type_to_str(found)));
}

#define SYMBOL_EXPECTED_TYPE(of, expected, found) symbol_expected_type(of, expected, found, base, result)

symbol_t *symbols_stack_resolve(symbols_stack_t *stack, str_slice_t *name) {
	SLICE_FOREACH(&stack->slice, symbol_t, symbol, {
		if (str_slice_equals(&symbol->name, name)) {
			return symbol;
		}
	});
	return NULL;
}

type_t symbols_type_of(symbols_stack_t *stack, str_slice_t *name, message_base_t base, result_t *result) {
	symbol_t *symbol = symbols_stack_resolve(stack, name); 
	if (!symbol) {
		PUSH_ERROR(error_undefined_symbol_new(error_base_new_simple(), *name));
		return NULL;
	}
	return symbol->type;
}

type_t symbols_check_expr(types_info_t *types, symbols_stack_t *stack, expr_t expr, message_base_t base, result_t *result);

type_t symbols_check_funcall(types_info_t *types, symbols_stack_t *stack, funcall_t *funcall, message_base_t base, result_t *result) {
	type_t type = symbols_type_of(stack, &funcall->ident.slice, base, result);
	if (!type) {
		return NULL;
	}
	if (type->kind != TYPE_FUNC) {
		printf("`???` is not a func!!!\n");
		PUSH_WARNING(WARNING_NOT_IMPLEMENTED);
		return NULL;
	}
	type_func_t *func = type_as_func(type);
	if (func->args.slice.len != funcall->args.slice.len) {
		PUSH_ERROR(error_invalid_func_args_count_new(error_base_new_simple(), funcall->ident.slice, func->args.slice.len, funcall->args.slice.len));
		return NULL;
	}
	SLICE_FOREACH(&funcall->args.slice, expr_t, arg, {
		typed_func_arg_t *targ = typed_func_args_slice_at(&func->args.slice, i);
		symbols_check_expr(types, stack, *arg, base, result);
	});
	return func->returns;
}

type_t symbols_check_expr(types_info_t *types, symbols_stack_t *stack, expr_t expr, message_base_t base, result_t *result) {
	if (!expr)  {
		return NULL;
	}
	SEALED_ASSERT_ALL_USED(expr, 4);
	switch (expr->kind) {
		case EXPR_IDENT: {
			expr_ident_t *expr_ident = expr_as_ident(expr);
			return symbols_type_of(stack, &expr_ident->ident.slice, base, result);
		}
		case EXPR_FUNCALL: {
			expr_funcall_t *expr_funcall = expr_as_funcall(expr);
			return symbols_check_funcall(types, stack, expr_funcall->funcall, base, result);	
		}
		case EXPR_CONST: {
			expr_const_t *expr_const = expr_as_const(expr);
			SEALED_ASSERT_ALL_USED(const, 2);
			const_t value = expr_const->value;
			switch (value->kind) {
				case CONST_INT: {
					return type_i32;
				}
				case CONST_STR: {
					return type_str;
				}
			}
		}
		case EXPR_BINOP: {
			expr_binop_t *expr_binop = expr_as_binop(expr);
			type_t ltype = symbols_check_expr(types, stack, expr_binop->left, base, result);
			type_t rtype = symbols_check_expr(types, stack, expr_binop->right, base, result);
			if (!ltype || !rtype) {
				return NULL;
			}
			if (ltype != rtype) {
				PUSH_ERROR(error_binop_invalid_types_new(error_base_new_simple(), type_to_str(ltype), type_to_str(rtype)));
				return NULL;
			}
			// TODO: unsupported binops
			binop_type_t btype = expr_binop->type;
			binop_type_t binary_binops[] = {	
 				BINOP_EQUALS,
				BINOP_NOT_EQUALS,
				BINOP_LESS,
				BINOP_GREATER,
				BINOP_LE,
				BINOP_GE
			};
#define BINARY_BINOPS_COUNT (sizeof(binary_binops) / sizeof(binary_binops[0]))
			for (int i = 0; i < BINARY_BINOPS_COUNT; i++) {
				if (binary_binops[i] == btype) {
					return type_bool;
				}
			}
			return ltype;
		}
	}
}

type_t symbols_check_definition(types_info_t *types, str_slice_t *name, symbols_stack_t *stack, definition_t *definition, message_base_t base, result_t *result) {
	type_t def_type = type_resolve_simple(types, &definition->type.slice);
	if (definition->expr) {
		type_t expr_type = symbols_check_expr(types, stack, definition->expr, base, result);
		if (def_type) {
			SYMBOL_EXPECTED_TYPE(*name, def_type, expr_type);
		}
	}
	return def_type;
}

void symbols_stack_push(symbols_stack_t *stack, symbol_t *symbol, message_base_t base, result_t *result) {
	if (symbols_stack_resolve(stack, &symbol->name)) {
		PUSH_ERROR(error_redefinition_new(error_base_new_simple(), symbol->name));
		return;
	}
	arr_push(stack, symbol);
}

#define SYMBOL_PUSH(symbol) symbols_stack_push(stack, symbol, base, result)

void symbols_check_stats(types_info_t *types, symbols_stack_t *stack, stats_t *stats, message_base_t base, result_t *result);

size_t symbols_check_stat(types_info_t *types, symbols_stack_t *stack, stat_t stat, message_base_t base, result_t *result) {
	SEALED_ASSERT_ALL_USED(stat, 4);
	switch (stat->kind) {
		case STAT_DEFINE: {
			stat_define_t *stat_define = stat_as_define(stat);
			type_t def_type = symbols_check_definition(types, &stat_define->name.slice, stack, stat_define->definition, base, result);
			symbol_t symbol = {
				.name = stat_define->name.slice,
				.type = def_type
			};
			SYMBOL_PUSH(&symbol);
			return 1;
		}
		case STAT_RETURN: {
			return 0;
		}
		case STAT_IF: {
			stat_if_t *stat_if = stat_as_if(stat);
			type_t cond_type = symbols_check_expr(types, stack, stat_if->if_cond_stat.cond, base, result);
			if (cond_type != type_bool) PUSH_ERROR(error_invalid_if_cond_type_new(error_base_new_simple(), type_to_str(cond_type)));
			symbols_check_stats(types, stack, &stat_if->if_cond_stat.stats, base, result);
			return 0;
		}
		case STAT_FUNCALL: {
			return 0;
		}
	}
}

void symbols_check_stats(types_info_t *types, symbols_stack_t *stack, stats_t *stats, message_base_t base, result_t *result) {
	size_t pushes = 0;
	SLICE_FOREACH(&stats->slice, stat_t, stat, {
		pushes += symbols_check_stat(types, stack, *stat, base, result);
	});
	// TODO: method pop_many
	for (size_t i = 0; i < pushes; i++) {
		arr_pop(stack);
	}
}

void symbols_push_func_arg(types_info_t *types, symbols_stack_t *stack, func_arg_t *arg, message_base_t base, result_t *result) {
	type_t arg_type = type_resolve_simple(types, &arg->type.slice);
	if (!arg_type) {
		return;
	}
	symbol_t arg_symbol = {
		.name = arg->name.slice,
		.type = arg_type
	};
	SYMBOL_PUSH(&arg_symbol);
}

void symbols_check_def(types_info_t *types, symbols_stack_t *stack, str_slice_t *name, def_content_t content, message_base_t base, result_t *result) {
	SEALED_ASSERT_ALL_USED(def_content, 2);
	switch (content->kind) {
		case DEF_CONTENT_FUNC: {
			def_content_func_t *def_func = def_content_as_func(content);
			SLICE_FOREACH(&def_func->args.slice, func_arg_t, arg, {
				symbols_push_func_arg(types, stack, arg, base, result);
			});
			symbols_check_stats(types, stack, &def_func->stats, base, result);
			for (size_t i = 0; i < def_func->args.slice.len; i++) {
				arr_pop(stack);
			}
			break;
		}
		case DEF_CONTENT_DEFINE: {
			def_content_define_t *def_define = def_content_as_define(content);
			symbols_check_definition(types, name, stack, def_define->definition, base, result);
			break;
		}
	}	
}

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
		symbols_check_def(types, &stack, &def->name.slice, def->content, base, result);
	});

	SLICE_FOREACH(&stack.slice, symbol_t, symbol, {
		printf("symbol `");
		str_slice_dump(&symbol->name, stdout);
		printf("`\n");
	});

	arr_free(&stack);	
}
