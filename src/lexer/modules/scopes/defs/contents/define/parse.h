#pragma once

#include "base/result.h"
#include "tokenizer/tokens.h"
#include "lexer/modules/scopes/defs/reader.h"
#include "../definition/parse.h"
#include <malloc.h>

def_read_result_t def_define_parse(tokens_slice_t *tokens, message_base_t base, result_t *result);