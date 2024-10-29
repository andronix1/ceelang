#pragma once

#include "lexer/projects/modules/scopes/exprs/parse.h"
#include "tokenizer/tokens.h"
#include "core/arr.h"
#include "define.h"
#include "../parse_result.h"

stat_parse_result_t define_stat_parse(tokens_slice_t tokens);