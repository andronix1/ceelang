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
    WARNING_NOT_IMPLEMENTED,
    warning_variants_count
} warning_type_t;

SEALED_CHILD_DEFINE(message, MESSAGE_WARNING, warning,
    (warning_type_t, type)
);

SEALED_KIND(error,
    ERROR_UNKNOWN_TOKEN,
    ERROR_MISSING_TOKEN,
    ERROR_INVALID_TOKEN,
    ERROR_SCOPE_NOT_CLOSED,
    ERROR_INVALID_EXPR_MISSING_BINOP,
    ERROR_INVALID_EXPR_BINOP,
    ERROR_STRING_NOT_CLOSED,
    ERROR_UNKNOWN_TYPE,
    ERROR_REDEFINITION,
    ERROR_INVALID_TYPE
);

SEALED_BASE(error);

SEALED_CHILD_DEFINE(error, ERROR_UNKNOWN_TYPE, unknown_type,
	(str_slice_t, of)
);

SEALED_CHILD_DEFINE(error, ERROR_REDEFINITION, redefinition,
	(str_slice_t, of)
);

SEALED_CHILD_DEFINE(error, ERROR_INVALID_TYPE, invalid_type,
	(str_slice_t, of),
	(str_t, expected),
	(str_t, found)
);

SEALED_CHILD_DEFINE(message, MESSAGE_ERROR, error,
    (error_t, error)
);

SEALED_FREE_DEFINE(message);

ARR_TYPED_ALIAS_DEFINE(messages, message_t);
