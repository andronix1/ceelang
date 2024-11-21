#include "executor.h"

vm_exec_ctx_t vm_create_ctx() {
	vm_exec_ctx_t result = {
		.ip = 0,
		.stack = malloc(sizeof(uint64_t) * 1024),
		.stack_top = 0
	};
	return result;
}

bool vm_exec_instr(vm_exec_ctx_t *ctx, vm_instr_t instr) {
	SEALED_ASSERT_ALL_USED(vm_instr, 11);
	switch (instr->kind) {
		case VM_INSTR_PUSHS: {
			vm_instr_pushs_t *pushs = vm_instr_as_pushs(instr);
			ctx->stack[ctx->stack_top] = ctx->stack[ctx->stack_top - 1 - pushs->rel_addr];
			ctx->stack_top++;
			ctx->ip += sizeof(vm_instr_pushs_t);
			break;
		}
		case VM_INSTR_PUSHI: {
			vm_instr_pushi_t *pushi = vm_instr_as_pushi(instr);
			ctx->stack[ctx->stack_top] = pushi->value;
			ctx->stack_top++;
			ctx->ip += sizeof(vm_instr_pushi_t);
			break;
		}
		case VM_INSTR_POP: {
			ctx->stack_top--;
			ctx->ip += sizeof(vm_instr_base_t);
			break;
		}
		case VM_INSTR_SUBI: {
			ctx->stack[ctx->stack_top - 2] -= ctx->stack[ctx->stack_top - 1];
			ctx->stack_top--;
			ctx->ip += sizeof(vm_instr_base_t);
			break;
		}
		case VM_INSTR_ADDI: {
			ctx->stack[ctx->stack_top - 2] += ctx->stack[ctx->stack_top - 1];
			ctx->stack_top--;
			ctx->ip += sizeof(vm_instr_base_t);
			break;
		}
		case VM_INSTR_MULI: {
			ctx->stack[ctx->stack_top - 2] *= ctx->stack[ctx->stack_top - 1];
			ctx->stack_top--;	
			ctx->ip += sizeof(vm_instr_base_t);
			break;
		}
		case VM_INSTR_DIVI: { 
			ctx->stack[ctx->stack_top - 2] /= ctx->stack[ctx->stack_top - 1];
			ctx->stack_top--;
			ctx->ip += sizeof(vm_instr_base_t);
			break;
		}
		case VM_INSTR_CALL: {
			vm_instr_call_t *call = vm_instr_as_call(instr);
			ctx->stack[ctx->stack_top] = ctx->ip + sizeof(vm_instr_call_t);
			ctx->stack_top++;
			ctx->ip = call->addr;
		       	break;	
		}
		case VM_INSTR_RET: {
			ctx->ip = ctx->stack[ctx->stack_top - 1];
			ctx->stack_top--;
		       	break;	
		}
		case VM_INSTR_RETV: {
			ctx->ip = ctx->stack[ctx->stack_top - 2];
			ctx->stack[ctx->stack_top - 2] = ctx->stack[ctx->stack_top - 1];
			ctx->stack_top--;
		       	break;	
		}
		case VM_INSTR_EXIT: {
			return false;
		}
	}
	return true;
}

size_t vm_write_instr(FILE *stream, vm_instr_t instr) {
	SEALED_ASSERT_ALL_USED(vm_instr, 11);
	size_t size = sizeof(vm_instr_base_t);
	switch (instr->kind) {
		case VM_INSTR_PUSHS: { size = sizeof(vm_instr_pushs_t); break; }
		case VM_INSTR_PUSHI: { size = sizeof(vm_instr_pushi_t); break; }
		case VM_INSTR_CALL:  { size = sizeof(vm_instr_call_t);  break; }
	}
	fwrite(instr, size, 1, stream);
	return size;
}

void vm_exec_instrs(vm_exec_ctx_t *ctx, void *instrs, size_t size) {
	while (ctx->ip < size && vm_exec_instr(ctx, instrs + ctx->ip)) {}
}
