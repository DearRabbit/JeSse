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

#define JSVM_LOCAL	(vmcode)jsvm_assign_local
#define JSVM_GLOBAL	(vmcode)jsvm_assign_global

#define JSVM_CALL	(vmcode)jsvm_callfunc

// for code like 'ffff ffff xxxx xxxx'
// the last 32 bit means the offset.
// pc += (offset+1)
#define JSVM_JUMP_MASK 0xffffffffL
#define _JSVM_JUMPCODE_TEST 0xffffffff00000000L

#define JSVM_CHECKJUMP(code) ((_JSVM_JUMPCODE_TEST&code) == _JSVM_JUMPCODE_TEST) 

#define JSVM_SetJump_Offset(line) ((_JSVM_JUMPCODE_TEST) | (JSVM_JUMP_MASK&line))
#define JSVM_GetJump_Offset(code) ((s32)(JSVM_JUMP_MASK&code))

int jsvm_Init(void);
void jsvm_Deinit(void);

int jsvm_execute(vmcode*);

int jsvm_assign_local(JsObject* name, JsObject* value);
int jsvm_assign_global(JsObject* name, JsObject* value);
int jsvm_callfunc(JsObject* name);

// WARNING: do not use it
void _jsvm_halt(void);