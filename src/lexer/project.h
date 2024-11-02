#pragma once

#include "core/str.h"
#include "modules/module.h"

typedef struct {
	str_t name;
	modules_t modules;
} project_t;
