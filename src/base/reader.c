#include "reader.h"

SEALED_CHILD_IMPL_FREE(read_result, ok, {
    if (self->free) {
        self->free(self->data);
    }
});
SEALED_CHILD_IMPL(read_result, invalid);

SEALED_FREE_IMPL(read_result, {
    SEALED_SHOULD_BE_FREED(read_result, READ_OK, ok)
}, 3);
