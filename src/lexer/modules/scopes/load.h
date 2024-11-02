#pragma once

#include <errno.h>
#include "tokenizer/tokenize.h"
#include "defs/parse.h"
#include "scope.h"

void scope_load(char *path, message_base_t base, result_t *result, scope_t *scope);