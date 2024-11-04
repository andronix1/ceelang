#pragma once

#include "../../../exprs/parse.h"
#include "base/reader.h"
#include "base/token_reader.h"
#include "definition.h"

READER_TYPE_ALIAS(definition, definition_t, NULL);

definition_read_result_t definition_parse(tokens_slice_t *tokens, message_base_t base, result_t *result, str_t *name);