#pragma once

#include "base/token_reader.h"
#include "../reader.h"

raw_expr_read_result_t raw_expr_scope_reader(tokens_slice_t *tokens, message_base_t base, result_t *result);