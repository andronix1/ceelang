#include "tasks.h"

char *next_arg(int *argc, char ***argv) {
	if (*argc == 0) {
		return NULL;
	}
	(*argc)--;
	char *result = (*argv)[0];
	*argv = &((*argv)[1]);
	return result;
}

task_err_t run_task(task_t *tasks, size_t tasks_len, int argc, char **argv) {
	next_arg(&argc, &argv);
	const char *task_name = next_arg(&argc, &argv);
	if (!task_name) {
		printf("ERROR: missing task name\n");
		return TASK_ERR_INVALID_USAGE;
	}
	for (size_t i = 0; i < tasks_len; i++) {
		task_t task = tasks[i];
		if (!strcmp(task.name, task_name)) {
			return task.func(argc, argv);
		}
	}
	printf("ERROR: unknown task \"%s\"\n", task_name);
	return TASK_OK;
}

