#pragma once

#include "lexer/projects/modules/scopes/exprs/parse.h"
#include "tokenizer/tokens.h"
#include "core/arr.h"
#include "funcall.h"
#include "../stat.h"

stat_t funcall_stat_parse(tokens_slice_t tokens);