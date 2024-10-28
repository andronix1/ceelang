#include "task.h"

task_err_t ast_task(int argc, char **argv) {
	const char *file_path = next_arg(&argc, &argv);
	if (!file_path) {
		printf("ERROR: missing file path\n");
		return TASK_ERR_INVALID_USAGE;
	}
	FILE *stream = fopen(file_path, "r");
	if (!stream) {
		printf("ERROR: failed to open file: %s\n", strerror(errno));
		return TASK_ERR_INTERNAL;
	}
	tokens_t tokens = tokenize(stream);
	fclose(stream);
	return TASK_OK;
}
