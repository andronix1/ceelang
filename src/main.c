#include <stdio.h>
#include <stdlib.h>

#include "tokenizer/task.h"
#include "lexer/task.h"
#include "tasks.h"

task_t tasks[] = {
	{
		.name = "tokens",
		.description = "show file tokens",
		.args = "<file>",
		.func = tokens_task
	},
	{
		.name = "ast",
		.description = "show file ast",
		.args = "<file>",
		.func = ast_task
	}
};
#define tasks_len sizeof(tasks) / sizeof(tasks[0])

void usage() {
	printf("usage: ./cee <task> [...args]\n");
	printf("tasks:\n");
	for (size_t i = 0; i < tasks_len; i++) {
		task_t task = tasks[i];
		printf("\t%s", task.name);
		if (task.args[0] != '\0') {
			printf(" %s", task.args);
		}
		printf(" - %s\n", task.description);
	}
}

int main(int argc, char **argv) {
	task_err_t err = run_task(tasks, tasks_len, argc, argv);
	switch (err) {
	case TASK_OK:
		break;
	case TASK_ERR_INVALID_USAGE:
		usage();
		return 1;
	case TASK_ERR_INTERNAL:
		printf("task finished due to internal error\n");
		return 1;
	default:
		printf("ERROR: unknown task err: %d\n", err);
	}
	return 0;
}
