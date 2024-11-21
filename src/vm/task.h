#pragma once

#include <errno.h>
#include "lexer/modules/scopes/load.h"
#include "debug/scope_print.h"
#include "tasks.h"
#include "analyzer/checks/types/check.h"
#include "analyzer/checks/symbols/check.h"
#include "executor/executor.h"

task_err_t vm_compile_task(int argc, char **argv);
