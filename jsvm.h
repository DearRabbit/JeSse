#pragma once

#define JSVM_HALT 	(vmcode)_jsvm_halt

#define JSVM_ADD	(vmcode)op_add
#define JSVM_SUB	(vmcode)op_sub
#define JSVM_MUL	(vmcode)op_mul
#define JSVM_DIV	(vmcode)op_div

#define JSVM_AND	(vmcode)op_and
#define JSVM_OR		(vmcode)op_or
#define JSVM_XOR	(vmcode)op_xor

#define JSVM_EQ		(vmcode)op_eq
#define JSVM_NEQ	(vmcode)op_neq
#define JSVM_LT		(vmcode)op_lt
#define JSVM_GT		(vmcode)op_gt
#define JSVM_LE		(vmcode)op_le
#define JSVM_GE		(vmcode)op_ge

#define JSVM_
#define JSVM_JUMP_MASK 0L

int jsvm_Init(void);
void jsvm_Deinit(void);

int jsvm_execute(vmcode*);
int jsvm_assign(JsObject*);

// WARNING: do not use it
void _jsvm_halt(void);