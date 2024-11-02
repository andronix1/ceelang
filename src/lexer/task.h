#pragma once

#include <string.h>
#include <errno.h>
#include "tasks.h"
#include "modules/scopes/load.h"
#include "tokenizer/tokenize.h"

task_err_t ast_task(int argc, char **argv);
