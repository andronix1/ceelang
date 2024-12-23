#pragma once

#include "base/result.h"
#include "tokenizer/tokens.h"
#include "lexer/modules/scopes/defs/reader.h"
#include "readers/reader.h"
#include "readers/return.h"
#include "readers/define.h"
#include "readers/if.h"
#include "readers/funcall.h"


void stats_parse(tokens_slice_t slice, message_base_t base, result_t *result, stats_t *stats);