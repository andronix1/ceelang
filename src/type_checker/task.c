#include "task.h"

task_err_t type_checker_task(int argc, char **argv) {
    char *file_path = next_arg(&argc, &argv);
	if (!file_path) {
		printf("ERROR: missing file path\n");
		return TASK_ERR_INVALID_USAGE;
	}

	result_t result = result_new();
	scope_t scope;
	message_base_t base = message_base_new_simple(str_slice_from_cstr(file_path), location_new(0, 0));
	scope_load(file_path, base, &result, &scope);

	result_print(&result);
    printf("NOT IMPLEMENTED YET!\n");

	if (result.success) {
		printf("Type checking finished!\n");
	} else {
		printf("Type checking failed!\n");
	}

	scope_free(&scope);
	result_free(&result);

	return TASK_OK;
}
