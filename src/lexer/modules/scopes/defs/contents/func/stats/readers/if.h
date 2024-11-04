#pragma once

#include "base/result.h"
#include "tokenizer/tokens.h"
#include "lexer/modules/scopes/defs/reader.h"
#include "lexer/modules/scopes/exprs/parse.h"
#include "../parse.h"

stat_read_result_t stat_if_parse(tokens_slice_t *tokens, message_base_t base, result_t *result);