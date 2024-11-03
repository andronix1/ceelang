#pragma once

#include "core/arr.h"
#include "core/str.h"
#include "core/sealed.h"
#include "base/location.h"

SEALED_KIND(message,
    MESSAGE_WARNING,
    MESSAGE_ERROR
);

SEALED_BASE(message,
    (str_slice_t, file),
    (location_t, location)
);

typedef enum {
    warning_variants_count
} warning_type_t;

SEALED_CHILD_DEFINE(message, MESSAGE_WARNING, warning,
    (warning_type_t, type)
);

typedef enum {
    ERROR_UNKNOWN_TOKEN,
    ERROR_MISSING_TOKEN,
    ERROR_INVALID_TOKEN,
    ERROR_SCOPE_NOT_CLOSED,
    error_variants_count
} error_type_t;

SEALED_CHILD_DEFINE(message, MESSAGE_ERROR, error,
    (error_type_t, type)
);

SEALED_FREE_DEFINE(message);

ARR_TYPED_ALIAS_DEFINE(messages, message_t);