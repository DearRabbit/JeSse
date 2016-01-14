#include "runtime.h"

#include <assert.h>

#ifndef JS_DEBUG
static 
#endif
JsObject* vmresult;

JsFuncObject* global_instance;
JsFuncObject* current_instance;

void
_jsvm_halt(void)
{
}

/*	if temp result is not referrence by other object,
 *	then put in trash bin
 */
#define _JS_EXECUTE_BINARY(binaryfunc) do					\
	{														\
		Js_XDECREF(vmresult);								\
		vmresult = binaryfunc((JsObject*)vm_set[pc+1],		\
		  (JsObject*)vm_set[pc+2]);							\
		pc += 3;											\
	} while (0)

#define _JS_EXECUTE_COMPARE(cmpfunc) do						\
	{														\
		_JS_EXECUTE_BINARY(cmpfunc);						\
		//Js_INCREF(vmresult);								\
	} while (0)

int
jsvm_execute(vmcode* vm_set)
{
	register ssize_t pc = 0;
	register vmcode func;

	while (pc >= 0)
	{
		func = vm_set[pc];

		// when the code < 0, it's in the operation set;
		// otherwise it's a function ptr of C
		if (func < 0 && JSVM_CHECKJUMP(func))
		{
			pc += (JSVM_GetJump_Offset(func) + 1);
		}
		else
		{
			if (func == JSVM_ADD)
				_JS_EXECUTE_BINARY(op_add);
			else if (func == JSVM_SUB)
				_JS_EXECUTE_BINARY(op_sub);
			else if (func == JSVM_EQ)
				_JS_EXECUTE_COMPARE(op_eq);
			else if (func == JSVM_HALT)
			{
				pc = -1;
				_jsvm_halt();
			}

			// less frequently
			else
				assert(0);
		}
	}
	return 0;
}

int jsvm_assign_local(JsObject* name, JsObject* value)
{
	return JsFunc_SetVariable(current_instance, name, value);
}
int jsvm_assign_global(JsObject* name, JsObject* value)
{
	return JsFunc_SetVariable(global_instance, name, value);
}
int jsvm_callfunc(JsObject* name)
{
	JsObject* funptr;
	funptr = JsFunc_GetVariable(current_instance, name);

	assert(funptr);
	if (JsDef_CheckType(funptr))
	{
		// try to new an instance
		funptr = JsDef_NewInstance(funptr);
	}
	if (JsFunc_CheckType(funptr))
	{
		// just call it if it's an instance.
		JsFunc_SetCallingRelation(current_instance, funptr);
		current_instance = (JsFuncObject*)funptr;
		jsvm_execute(JsFunc_GetCode(funptr));

		// WARING&TODO: to pass argument
		return 0;
	}

	assert(0);
	return -1;
}

int
jsvm_Init(void)
{
	// unusual way of initialization.
	JsFuncObject* alloc = Js_Malloc(sizeof(JsFuncObject));
	global_instance = alloc;
	current_instance = alloc;

	JsObject_INIT(alloc, &JsFunc_Type);
	// no need to create a global definition.
	alloc->func_def = NULL;
	alloc->var_table = (JsDictObject*)JsDict_New();

	return 0;
}

void
jsvm_Deinit(void)
{

}