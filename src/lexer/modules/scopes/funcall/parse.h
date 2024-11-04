#pragma once

#include "../exprs/parse.h"
#include "base/token_reader.h"
#include "funcall.h"

READER_TYPE_ALIAS(funcall, funcall_t, NULL)

funcall_read_result_t funcall_reader(tokens_slice_t *tokens, message_base_t base, result_t *result);