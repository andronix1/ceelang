#pragma once

#include "tokenizer/tokens.h"
#include "define/parse.h"
#include "funcall/parse.h"
#include "return/parse.h"
#include "if/parse.h"
#include "parse_result.h"
#include "stat.h"

stat_parse_result_t stat_parse(tokens_slice_t tokens);
stats_t stats_parse(tokens_slice_t tokens);