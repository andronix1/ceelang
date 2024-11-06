#pragma once

#include <errno.h>
#include "lexer/modules/scopes/load.h"
#include "debug/scope_print.h"
#include "tasks.h"
#include "checks/types/check.h"
#include "checks/symbols/check.h"

task_err_t type_checker_task(int argc, char **argv);
