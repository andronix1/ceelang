#pragma once

#include "lexer/projects/modules/scopes/exprs/parse.h"
#include "tokenizer/tokens.h"
#include "core/arr.h"
#include "define.h"
#include "../stat.h"

stat_t define_stat_parse(slice_t tokens);