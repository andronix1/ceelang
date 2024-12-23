#pragma once

#include "base/result.h"
#include "tokenizer/tokens.h"
#include "lexer/modules/scopes/defs/reader.h"
#include "stats/parse.h"
#include <malloc.h>

def_read_result_t def_fun_parse(tokens_slice_t *tokens, message_base_t base, result_t *result);