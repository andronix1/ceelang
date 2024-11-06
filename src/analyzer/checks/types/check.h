#pragma once

#include "lexer/modules/scopes/scope.h"
#include "base/result.h"
#include "type.h"

void types_check_stat(types_info_t *types, stat_t stat, message_base_t base, result_t *result);
void types_check_scope(types_info_t *types, scope_t *scope, message_base_t base, result_t *result);
