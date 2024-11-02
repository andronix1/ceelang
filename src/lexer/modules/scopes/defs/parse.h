#pragma once

#include "reader.h"
#include "base/result.h"
#include "tokenizer/tokens.h"
#include "def.h"

void defs_parse(tokens_slice_t tokens, message_base_t base, result_t *result, defs_t *defs);