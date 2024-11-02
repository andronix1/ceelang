#include "task.h"

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
			printf(" {}");
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
	str_slice_t file_path_slice = str_slice_from_cstr(file_path);

	result_t result = result_new();
	scope_t scope;
	message_base_t base = message_base_new_simple(str_copy_from_slice(&file_path_slice), location_new(0, 0));
	scope_load(file_path, base, &result, &scope);

	result_print(&result);
	printf("ast built:\n");
	SLICE_FOREACH(&scope.defs.slice, def_t, def, {
		print_def(*def);
	});
	printf("\n-------------\n");

	if (result.success) {
		printf("AST building finished!\n");
	} else {
		printf("AST building failed!\n");
	}

	scope_free(&scope);
	message_free_base(&base);
	result_free(&result);

	return TASK_OK;
}
