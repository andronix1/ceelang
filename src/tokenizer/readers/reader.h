#pragma once

#include <stddef.h>
#include <stdbool.h>
#include "core/sealed.h"
#include "../tokens.h"
#include "base/messages.h"

SEALED_KIND(token_read_result,
    TOKEN_READ_OK,
    TOKEN_READ_NOT_THIS,
    TOKEN_READ_ERR_INVALID
);

SEALED_BASE(token_read_result);

SEALED_CHILD_DEFINE_FREE(token_read_result, TOKEN_READ_OK, ok,
    SEALED_PARAM(token_t, token),
    SEALED_PARAM(size_t, len),
    SEALED_PARAM(bool, has_warning),
    SEALED_PARAM(warning_type_t, warning)
);

SEALED_CHILD_DEFINE(token_read_result, TOKEN_READ_ERR_INVALID, invalid,
    SEALED_PARAM(size_t, len),
    SEALED_PARAM(error_type_t, error)
);

SEALED_FREE_DEFINE(token_read_result);

typedef token_read_result_t (*token_reader_t)(str_slice_t *slice);