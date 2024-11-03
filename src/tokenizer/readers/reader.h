#pragma once

#include <stddef.h>
#include <stdbool.h>
#include "core/sealed.h"
#include "../tokens.h"
#include "base/messages.h"
#include "base/reader.h"

READER_TYPE_ALIAS(token, token_base_t, NULL);

typedef token_read_result_t (*token_reader_t)(str_slice_t *slice);