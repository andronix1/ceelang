#pragma once

#include "expr.h"
#include "lexer/base.h"

typedef enum {
	STAT_ASSIGN,
	STAT_DECLARE,
	STAT_COND,
} stat_type_t;

typedef struct {
	stat_type_t type;
} stat_base_t;

typedef stat_base_t* stat_t;

typedef struct {
	stat_t stats;
} code_node_t;
