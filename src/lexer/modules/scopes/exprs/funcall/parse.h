#pragma once

#include "funcall.h"
#include "tokenizer/tokens.h"
#include "../parse.h"

funcall_t funcall_parse(tokens_slice_t tokens);