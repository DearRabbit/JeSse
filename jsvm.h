#include "runtime.h"

#define JSVM_HALT 	(vmcode)_jsvm_halt

#define JSVM_ADD	(vmcode)op_add
#define JSVM_SUB	(vmcode)op_sub
#define JSVM_MUL	(vmcode)op_mul
#define JSVM_DIV	(vmcode)op_div

#define JSVM_AND	(vmcode)op_and
#define JSVM_OR		(vmcode)op_or
#define JSVM_XOR	(vmcode)op_xor

void _jsvm_halt(void);
int jsvm_execute(vmcode*);