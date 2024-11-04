#pragma once

#include "core/str.h"
#include "chars.h"
#include "reader.h"

token_read_result_t str_reader(str_slice_t *slice, message_base_t base, result_t *result);