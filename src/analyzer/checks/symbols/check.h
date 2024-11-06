#pragma once

#include "lexer/modules/scopes/scope.h"
#include "base/result.h"
#include "symbol.h"

ARR_TYPED_ALIAS_DEFINE(symbols_stack, symbol_t);

void symbols_check_scope(types_info_t *types, scope_t *scope, message_base_t base, result_t *result);
