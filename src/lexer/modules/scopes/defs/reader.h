#pragma once

#include <stddef.h>
#include <stdbool.h>
#include "tokenizer/tokens.h"
#include "core/sealed.h"
#include "def.h"
#include "base/result.h"

SEALED_KIND(def_read_result,
    DEF_READ_OK,
    DEF_READ_NOT_THIS,
    DEF_READ_ERR_INVALID
);

SEALED_BASE(def_read_result);

SEALED_CHILD_DEFINE_FREE(def_read_result, DEF_READ_OK, ok,
    SEALED_PARAM(def_t, def),
    SEALED_PARAM(size_t, len)
);

SEALED_CHILD_DEFINE(def_read_result, DEF_READ_ERR_INVALID, invalid,
    SEALED_PARAM(size_t, len)
);

SEALED_FREE_DEFINE(def_read_result);

typedef def_read_result_t (*def_reader_t)(tokens_slice_t *tokens, message_base_t base, result_t *result);