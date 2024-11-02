#include "reader.h"

SEALED_CHILD_IMPL_FREE(def_read_result, ok, { });
SEALED_CHILD_IMPL(def_read_result, invalid);

SEALED_FREE_IMPL(def_read_result, {
    SEALED_SHOULD_BE_FREED(def_read_result, DEF_READ_OK, ok)
}, 3);