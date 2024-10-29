#pragma once

#include "lexer/projects/modules/scopes/exprs/parse.h"
#include "tokenizer/tokens.h"
#include "core/arr.h"
#include "if.h"
#include "../parse.h"
#include "../parse_result.h"

typedef struct {
    size_t len;
    stat_t stat;
} if_parse_result_t;

stat_parse_result_t if_stat_parse(tokens_slice_t tokens);