#include "task.h"

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
			printf(")\n");
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
