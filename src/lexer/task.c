#include "task.h"

void print_expr(expr_t expr);

void print_funcall(funcall_t *funcall) {
	str_slice_dump(&funcall->ident.slice, stdout);
	printf("(");
	SLICE_FOREACH(&funcall->args.slice, expr_t, expr, {
		if (i != 0) {
			printf(", ");
		}
		print_expr(*expr);
	});
	printf(")");
}

void print_const(const_t data) {
	SEALED_ASSERT_ALL_USED(const, 2);
	switch (data->kind) {
		case CONST_INT:
			printf("%d", const_as_int(data)->value);
			break;
		case CONST_STR:
			printf("\"");
			str_slice_dump(&const_as_str(data)->value.slice, stdout);
			printf("\"");
			break;
	}
}

void print_expr(expr_t expr) {
	SEALED_ASSERT_ALL_USED(expr, 4);
	switch (expr->kind) {
		case EXPR_IDENT: {
			expr_ident_t *expr_ident = expr_as_ident(expr);
			str_slice_dump(&expr_ident->ident.slice, stdout);
			break;
		}
		case EXPR_BINOP: {
			SEALED_ASSERT_ALL_USED(binop, 6);
			const char *binops[] = { "+", "-", "*", "/", "==", "!=" };
			expr_binop_t *expr_binop = expr_as_binop(expr);
			printf("(");
			print_expr(expr_binop->left);
			printf(" %s ", binops[expr_binop->type]);
			print_expr(expr_binop->right);
			printf(")");
			break;
		}
		case EXPR_CONST: {
			expr_const_t *expr_const = expr_as_const(expr);
			print_const(expr_const->value);
			break;
		}
		case EXPR_FUNCALL: {
			expr_funcall_t *expr_funcall = expr_as_funcall(expr);
			print_funcall(expr_funcall->funcall);
			break;
		}
		default:
			printf("???");
			break;
	}
}

void print_stat(stat_t stat) {
	SEALED_ASSERT_ALL_USED(stat, 4);
	switch (stat->kind) {
		case STAT_RETURN: {
			stat_return_t *stat_return = stat_as_return(stat);
			printf("return");
			if (stat_return->expr) {
				printf(" ");
				print_expr(stat_return->expr);
			}
			break;
		}
		case STAT_FUNCALL: {
			stat_funcall_t *stat_funcall = stat_as_funcall(stat);
			print_funcall(stat_funcall->funcall);
			break;
		}
		case STAT_DEFINE: {
			stat_define_t *stat_define = stat_as_define(stat);
			switch (stat_define->define_type) {
				case STAT_DEFINE_CONST:
					printf("const");
					break;
				case STAT_DEFINE_VAR:
					printf("var");
					break;
			}
			printf(" ");
			str_slice_dump(&stat_define->name.slice, stdout);
			printf(": ");
			str_slice_dump(&stat_define->type.slice, stdout);
			if (stat_define->expr) {
				printf(" = ");
				print_expr(stat_define->expr);
			}
			break;
		}
		default:
			printf("???");
			break;
	}
}

void print_def(def_t def) {
	SEALED_ASSERT_ALL_USED(def_content, 1);
	switch (def.content->kind) {
		case DEF_CONTENT_FUNC: {
			def_content_func_t *func = def_content_as_func(def.content);
			printf("func ");
			str_slice_dump(&def.name.slice, stdout);
			printf("(");
			for (size_t i = 0; i < func->args.slice.len; i++) {
				if (i != 0) {
					printf(", ");
				}
				func_arg_t *arg = func_args_slice_at(&func->args.slice, i);
				str_slice_dump(&arg->name.slice, stdout);
				printf(": ");
				str_slice_dump(&arg->type.slice, stdout);
			}
			printf(")");
			if (func->return_type) {
				printf(": ");
				str_slice_dump(&func->return_type->slice, stdout);
			}
			printf(" {");
			SLICE_FOREACH(&func->stats.slice, stat_t, stat, {
				printf(" ");
				print_stat(*stat);
				printf(";");
			});
			printf(" }");
			break;
		}
		// case DEF_CONTENT_CONST:
		// 	printf("const");
		// 	break;
	}
}

task_err_t ast_task(int argc, char **argv) {
	char *file_path = next_arg(&argc, &argv);
	if (!file_path) {
		printf("ERROR: missing file path\n");
		return TASK_ERR_INVALID_USAGE;
	}

	result_t result = result_new();
	scope_t scope;
	message_base_t base = message_base_new_simple(str_slice_from_cstr(file_path), location_new(0, 0));
	scope_load(file_path, base, &result, &scope);

	printf("-------------\n");
	result_print(&result);
	printf("-------------\n");
	SLICE_FOREACH(&scope.defs.slice, def_t, def, {
		print_def(*def);
		printf("\n");
	});
	printf("-------------\n");

	if (result.success) {
		printf("AST building finished!\n");
	} else {
		printf("AST building failed!\n");
	}

	scope_free(&scope);
	result_free(&result);

	return TASK_OK;
}
