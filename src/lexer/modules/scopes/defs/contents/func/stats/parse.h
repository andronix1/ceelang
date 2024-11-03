#pragma once

#include "base/result.h"
#include "tokenizer/tokens.h"
#include "lexer/modules/scopes/defs/reader.h"

READER_TYPE_ALIAS(stat, stat_t, free);

void stats_parse(tokens_slice_t slice, message_base_t base, result_t *result, stats_t *stats);