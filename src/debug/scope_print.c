#include "scope_print.h"

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
			SEALED_ASSERT_ALL_USED(binop, 10);
			const char *binops[] = { "+", "-", "*", "/", "==", "!=", "<", ">", "<=", ">=" };
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

void print_definition(definition_t *definition, str_t *name) {
	switch (definition->kind) {
		case DEFINITION_CONST:
			printf("const");
			break;
		case DEFINITION_VAR:
			printf("var");
			break;
	}
	printf(" ");
	str_slice_dump(&name->slice, stdout);
	printf(": ");
	str_slice_dump(&definition->type.slice, stdout);
	if (definition->expr) {
		printf(" = ");
		print_expr(definition->expr);
	}
	printf(";");
}

#define TAB() for (size_t j = 0; j < level; j++) { printf("    "); }

void print_stats(stats_slice_t *stats, size_t level);

void print_stat(stat_t stat, size_t level) {
	TAB();
	SEALED_ASSERT_ALL_USED(stat, 4);
	switch (stat->kind) {
		case STAT_RETURN: {
			stat_return_t *stat_return = stat_as_return(stat);
			printf("return");
			if (stat_return->expr) {
				printf(" ");
				print_expr(stat_return->expr);
			}
			printf(";");
			break;
		}
		case STAT_FUNCALL: {
			stat_funcall_t *stat_funcall = stat_as_funcall(stat);
			print_funcall(stat_funcall->funcall);
			printf(";");
			break;
		}
		case STAT_DEFINE: {
			stat_define_t *stat_define = stat_as_define(stat);
			print_definition(stat_define->definition, &stat_define->name);
			break;
		}
		case STAT_IF: {
			stat_if_t *stat_if = stat_as_if(stat);
			printf("if ");
			print_expr(stat_if->if_cond_stat.cond);
			printf(" {\n");
			print_stats(&stat_if->if_cond_stat.stats.slice, level + 1);
			TAB();
			printf("}");
			SLICE_FOREACH(&stat_if->elifs.slice, if_cond_stat_t, stat, {
				printf(" elif ");
				print_expr(stat->cond);
				printf(" {\n");
				print_stats(&stat->stats.slice, level + 1);
				TAB();
				printf("}");
			});
			if (stat_if->else_stats) {
				printf(" else {\n");
				print_stats(&stat_if->else_stats->slice, level + 1);
				TAB();
				printf("}");
			}
			break;
		}
		default:
			printf("???");
			break;
	}
}

void print_stats(stats_slice_t *stats, size_t level) {
	SLICE_FOREACH(stats, stat_t, stat, {
		print_stat(*stat, level);
		printf("\n");
	});
}

void print_def(def_t def) {
	SEALED_ASSERT_ALL_USED(def_content, 2);
	switch (def.content->kind) {
		case DEF_CONTENT_FUNC: {
			def_content_func_t *func = def_content_as_func(def.content);
			printf("fun ");
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
			printf(" {\n");
			print_stats(&func->stats.slice, 1);
			printf("}");
			break;
		}
		case DEF_CONTENT_DEFINE: {
			def_content_define_t *define = def_content_as_define(def.content);
			print_definition(define->definition, &def.name);
			break;
		}
	}
}

void print_scope(scope_t *scope) {
	printf("scope `");
	str_slice_dump(&scope->name.slice, stdout);
	printf("`:\n");
	printf("-------------\n");
	SLICE_FOREACH(&scope->defs.slice, def_t, def, {
		print_def(*def);
		printf("\n\n");
	});
	printf("-------------\n");
}