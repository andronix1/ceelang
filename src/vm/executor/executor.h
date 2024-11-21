#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <malloc.h>
#include "vm/bytecode/instr.h"

typedef struct {
	size_t ip;
	uint64_t *stack;
	size_t stack_top;
} vm_exec_ctx_t;

vm_exec_ctx_t vm_create_ctx();
size_t vm_write_instr(FILE *stream, vm_instr_t instr);
void vm_exec_instrs(vm_exec_ctx_t *ctx, void *instrs, size_t size);
