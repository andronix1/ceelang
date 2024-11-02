#pragma once

#include <stdbool.h>
#include "base/result.h"
#include "readers/reader.h"
#include "readers/keywords.h"
#include "readers/symbols.h"
#include "readers/uint.h"
#include "readers/ident.h"
#include "tokens.h"

void tokenize(FILE* stream, message_base_t base, result_t *result, tokens_t *tokens);