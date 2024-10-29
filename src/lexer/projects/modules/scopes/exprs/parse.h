#pragma once

#include "binop.h"
#include "lexer/projects/modules/scopes/exprs/funcall/parse.h"
#include "tokenizer/tokens.h"
#include "funcall/funcall.h"
#include "consts/integer.h"
#include "ident.h"
#include "expr.h"

expr_t expr_parse(tokens_slice_t tokens);