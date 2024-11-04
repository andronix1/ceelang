#pragma once

#include "expr.h"
#include "base/reader.h"
#include "readers/ident.h"
#include "readers/operator.h"
#include "readers/int.h"
#include "readers/scope.h"
#include "readers/str.h"
#include "readers/fun.h"
#include "raw_expr.h"

void raw_expr_parse(tokens_slice_t slice, message_base_t base, result_t *result, raw_expr_t *raw_expr);
void expr_parse(tokens_slice_t slice, message_base_t base, result_t *result, expr_t *expr);
