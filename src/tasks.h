#pragma once

#include <stdio.h>
#include <string.h>

typedef enum {
	TASK_OK = 0,
	TASK_ERR_INVALID_USAGE,
	TASK_ERR_INTERNAL
} task_err_t;

typedef task_err_t (*task_func_t)(int, char**);

typedef struct {
	const char *name;
	const char *description;
	const char *args;
	task_func_t func;
} task_t;

char *next_arg(int *argc, char ***argv);
task_err_t run_task(task_t *tasks, size_t tasks_len, int argc, char **argv);
