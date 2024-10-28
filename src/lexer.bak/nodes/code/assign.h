#pragma once

typedef struct {
	stat_type_t type = STAT_ASSIGN;
	ident_t name;
	expr_t expr;
} stat_assign_t;
