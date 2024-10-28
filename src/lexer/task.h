#pragma once

#include <string.h>
#include <errno.h>
#include "tasks.h"
#include "tokenizer/tokenize.h"
#include "projects/modules/scopes/load.h"

task_err_t ast_task(int argc, char **argv);
