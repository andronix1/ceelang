#include "task.h"

task_err_t vm_compile_task(int argc, char **argv) {
	char *src_file_path = next_arg(&argc, &argv);
	char *dst_file_path = next_arg(&argc, &argv);	

	if (!src_file_path || !dst_file_path) {
		printf("ERROR: missing file path\n");
		return TASK_ERR_INVALID_USAGE;
	}

	result_t result = result_new();
	scope_t scope;
	message_base_t base = message_base_new_simple(str_slice_from_cstr(src_file_path), location_new(0, 0));
	scope_load(src_file_path, base, &result, &scope);
	
	types_info_t info = types_info_new();
	types_check_scope(&info, &scope, base, &result);
	symbols_check_scope(&info, &scope, base, &result);
	result_print(&result);
	
	scope_free(&scope);
	result_free(&result);
	{
		FILE *out_file = fopen(dst_file_path, "w");
		vm_bc_builder_t builder = vm_bc_builder_new_with_cap(1);
		size_t ip = 0;
		size_t add3_addr = 44;

		ip += vm_write_instr(out_file, (vm_instr_t)vm_instr_pushi_new(vm_instr_base_new_simple(), 1));
		ip += vm_write_instr(out_file, (vm_instr_t)vm_instr_pushi_new(vm_instr_base_new_simple(), 2));
		ip += vm_write_instr(out_file, (vm_instr_t)vm_instr_pushi_new(vm_instr_base_new_simple(), 3));
		ip += vm_write_instr(out_file, (vm_instr_t)vm_instr_call_new(vm_instr_base_new_simple(), add3_addr));
		ip += vm_write_instr(out_file, (vm_instr_t)vm_instr_base_new(VM_INSTR_EXIT));
		printf("add3: %d\n", ip);
		assert(ip == add3_addr);
		ip += vm_write_instr(out_file, (vm_instr_t)vm_instr_pushs_new(vm_instr_base_new_simple(), 1));
		ip += vm_write_instr(out_file, (vm_instr_t)vm_instr_pushs_new(vm_instr_base_new_simple(), 3));
		ip += vm_write_instr(out_file, (vm_instr_t)vm_instr_base_new(VM_INSTR_ADDI));
		ip += vm_write_instr(out_file, (vm_instr_t)vm_instr_pushs_new(vm_instr_base_new_simple(), 4));
		ip += vm_write_instr(out_file, (vm_instr_t)vm_instr_base_new(VM_INSTR_ADDI));
		ip += vm_write_instr(out_file, (vm_instr_t)vm_instr_base_new(VM_INSTR_RETV));

		fclose(out_file);
	}
	
	{
		FILE *out_file = fopen(dst_file_path, "r");
		fseek(out_file, 0, SEEK_END);
		size_t size = ftell(out_file);
		fseek(out_file, 0, SEEK_SET);
		void *instrs = malloc(size);
		fread(instrs, 1, size, out_file);
		fclose(out_file);
		vm_exec_ctx_t ctx = vm_create_ctx();
		vm_exec_instrs(&ctx, instrs, size);
		for (size_t i = 0; i < ctx.stack_top; i++) {
			printf("%d ", ctx.stack[i]);
		}
		printf("\n");
	}
	return TASK_OK;
}
