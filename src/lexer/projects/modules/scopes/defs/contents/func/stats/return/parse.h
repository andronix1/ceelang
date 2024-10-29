#pragma once

#include "lexer/projects/modules/scopes/exprs/parse.h"
#include "tokenizer/tokens.h"
#include "core/arr.h"
#include "return.h"
#include "../parse_result.h"

stat_parse_result_t return_stat_parse(tokens_slice_t tokens);