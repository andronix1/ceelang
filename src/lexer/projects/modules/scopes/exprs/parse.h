#pragma once

#include "binop.h"
#include "tokenizer/tokens.h"
#include "funcall.h"
#include "ident.h"
#include "expr.h"

expr_t expr_parse(tokens_slice_t tokens);