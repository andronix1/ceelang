#include "task.h"

void print_expr(expr_t expr, bool top);

void print_funcall(funcall_t *funcall) {
	str_slice_dump(&funcall->ident.slice, stdout);
	printf("(");
	bool need_comma = false;
	arr_foreach(funcall_arg_t, &funcall->args, arg, {
		if (need_comma) {
			printf(", ");
		} else {
			need_comma = true;
		}
		print_expr(*arg, true);
	});
	printf(")");
}

void print_expr(expr_t expr, bool top) {
	switch (expr->type) {
		case EXPR_BINOP: {
			if (!top) {
				printf("(");
			}
			expr_binop_t *binop = (expr_binop_t*)expr;
			print_expr(binop->left, false);
			switch (binop->type) {
				case EXPR_BINOP_DIVIDE:
					printf(" / ");
					break;
				case EXPR_BINOP_MULTIPLY:
					printf(" * ");
					break;
				case EXPR_BINOP_PLUS:
					printf(" + ");
					break;
				case EXPR_BINOP_MINUS:
					printf(" - ");
					break;
				case EXPR_BINOP_EQUALS:
					printf(" == ");
					break;
				case EXPR_BINOP_NOT_EQUALS:
					printf(" != ");
					break;
				default:
					printf(" ??? ");
					break;
			}
			print_expr(binop->right, false);
			if (!top) {
				printf(")");
			}
			break;
		}
		case EXPR_CONST: {
			expr_const_t const_expr = (expr_const_t)expr;
			switch (const_expr->type) {
				case EXPR_CONST_UINT: {
					expr_const_integer_t *int_const = (expr_const_integer_t*)expr;
					printf("%d", int_const->value);
					break;
				}
				case EXPR_CONST_STR: {
					expr_const_str_t *str_const = (expr_const_str_t*)expr;
					fputc('\"', stdout);
					str_slice_dump(&str_const->value.slice, stdout);
					fputc('\"', stdout);
					break;
				}
				default:
					printf("???");
					break;
			}
			break;
		}
		case EXPR_FUNCALL: {
			expr_funcall_t *funcall = (expr_funcall_t*)expr;
			print_funcall(&funcall->funcall);
			break;
		}
		case EXPR_IDENT:
			str_slice_dump(&((expr_ident_t*)expr)->ident.slice, stdout);
			break;
	}
}

void print_stat(stat_t src_stat) {
	switch (src_stat->type) {
		case STAT_DEFINE: {
			stat_define_t *stat = (stat_define_t*)src_stat;
			printf("\t%s ", stat->define_type == STAT_DEFINE_CONST ? "const" : "var");
			str_slice_dump(&stat->name.slice, stdout);
			printf(": ");
			str_slice_dump(&stat->type.slice, stdout);
			if (stat->expr) {
				printf(" = ");
				print_expr(stat->expr, true);
			}
			printf(";\n");
			break;
		}
		case STAT_FUNCALL: {
			stat_funcall_t *stat = (stat_funcall_t*)src_stat;
			printf("\t");
			print_funcall(&stat->funcall);	
			printf(";\n");
			break;
		}
		case STAT_IF: {
			stat_if_t *stat = (stat_if_t*)src_stat;
			printf("\tif ");
			print_expr(stat->cond, true);
			printf(" {\n");
			stats_t if_stats = stat->if_stats;
			arr_foreach(stat_t, &if_stats, stat, {
				printf("\t");
				print_stat(*stat);
			});
			printf("\t}\n");
			break;
		}
		case STAT_RETURN: {
			stat_return_t *stat = (stat_return_t*)src_stat;
			printf("\treturn");
			if (stat->expr) {
				printf(" ");
				print_expr(stat->expr, true);
			}
			printf(";\n");
			break;
		}
		default:
			printf("???\n");
			break;
	}
}

void print_def(def_t def) {
	switch (def.content->type) {
		case DEF_CONTENT_FUNC: {
			def_content_func_t func = *(def_content_func_t*)def.content;
			str_push(&def.name, '\0');
			printf("fun %s(", def.name.slice.ptr);
			for (size_t i = 0; i < func.args.slice.len; i++) {
				func_arg_t arg = slice_at(func_arg_t, &func.args.slice, i);
				if (i != 0) {
					printf(", ");
				}
				str_push(&arg.name, '\0');
				str_push(&arg.type, '\0');
				printf("%s: %s", arg.name.slice.ptr, arg.type.slice.ptr);
			}
			printf(")");
			if (func.return_type) {
				printf(": ");
				str_slice_dump(&func.return_type->slice, stdout);
			}
			printf(" {");
			if (func.stats.slice.len != 0) {
				fputc('\n', stdout);
				arr_foreach(stat_t, &func.stats, stat, {
					print_stat(*stat);
				});
			}
			printf("}\n");
			break;
		}
		case DEF_CONTENT_CONST:
			str_push(&def.name, '\0');
			printf("const %s\n", def.name.slice.ptr);
			break;
	}
}

task_err_t ast_task(int argc, char **argv) {
	char *file_path = next_arg(&argc, &argv);
	if (!file_path) {
		printf("ERROR: missing file path\n");
		return TASK_ERR_INVALID_USAGE;
	}
	scope_t scope = scope_load(file_path);
	arr_foreach(def_t, &scope.defs, def, {
		print_def(*def);
	});
	return TASK_OK;
}
