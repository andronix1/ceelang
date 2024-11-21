#include "core/sealed.h"
#include "core/arr.h"

/* SOURCE
 * -------------------------
 * fun to_percents(a: i32, b: i32, v: i32) {
 *	return (b - a) * 100 / v;
 * }
 *
 * fun main() {
 *	const v: i32 = to_percents(10, 20, 12);
 * }
 * -------------------------
 * BYTECODE 
 * -------------------------
 * to_percents:			# stack: a, b, v, FROM
 *	PUSHS 2			# stack: a, b, v, FROM, b
 *	PUSHS 4			# stack: a, b, v, FROM, b, a
 * 	SUBI			# stack: a, b, v, FROM, b - a
 * 	PUSHI 100		# stack: a, b, v, FROM, b - a, 100
 * 	MULI 			# stack: a, b, v, FROM, b - a * 100
 * 	PUSHS 2 		# stack: a, b, v, FROM, b - a * 100, v
 * 	DIVI	 		# stack: a, b, v, FROM, ((b - a) * 100) / v
 * 	RETI	 		# stack: a, b, v 
 *
 * main:			# stack: 
 * 	PUSHI 10 		# stack: 10
 * 	PUSHI 20 		# stack: 10, 20
 * 	PUSHI 12 		# stack: 10, 20, 12
 * 	CALL to_percents 	# stack: 10, 20, 12
 * 	POP 			# stack: 10, 20
 * 	POP 			# stack: 10
 * 	POP	 		# stack:
 * 	EXIT
 * -------------------------
 * */

SEALED_KIND(vm_instr,
	VM_INSTR_PUSHS,		// push to stack from stack
	VM_INSTR_PUSHI,		// push value to stack
	VM_INSTR_POP,		// pop stack
	VM_INSTR_SUBI,		// subtract
	VM_INSTR_ADDI,		// add 
	VM_INSTR_MULI,		// multiply
	VM_INSTR_DIVI,		// divide
	VM_INSTR_CALL,		// call
	VM_INSTR_RETV,		// return value from stack
	VM_INSTR_RET,		// return
	VM_INSTR_EXIT		// exit from program
);

SEALED_BASE(vm_instr);

SEALED_CHILD_DEFINE(vm_instr, VM_INSTR_PUSHS, pushs,
	(size_t, rel_addr)
);

SEALED_CHILD_DEFINE(vm_instr, VM_INSTR_PUSHI, pushi,
	(int, value)
);

SEALED_CHILD_DEFINE(vm_instr, VM_INSTR_CALL, call,
	(size_t, addr)
);

ARR_TYPED_ALIAS_DEFINE(vm_bc_builder, vm_instr_t);
