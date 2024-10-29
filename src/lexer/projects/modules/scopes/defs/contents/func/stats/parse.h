#pragma once

#include "tokenizer/tokens.h"
#include "define/parse.h"
#include "funcall/parse.h"
#include "stat.h"

stat_t stat_parse(tokens_slice_t tokens);