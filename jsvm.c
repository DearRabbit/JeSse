#include "jsvm.h"

#include <assert.h>

#ifdef JS_DEBUG
JsObject* vmresult;
#endif

void
_jsvm_halt(void)
{
}

#define _JS_EXECUTE_BINARY(binaryfunc) do					\
	{														\
		vmresult = binaryfunc((JsObject*)vm_set[pc+1],		\
			(JsObject*)vm_set[pc+2]);						\
		pc += 3;											\
	} while (0)

int jsvm_execute(vmcode* vm_set)
{
	register ssize_t pc = 0;
	register vmcode func;
	vmcode op1, op2;

	while (pc >= 0)
	{
		func = vm_set[pc];

		// when the code < 0, it's in the operation set;
		// otherwise it's a function ptr of C
		if ((long)func < 0)
		{

		}
		else
		{
			if (func == JSVM_ADD)
				_JS_EXECUTE_BINARY(op_add);
			else if (func == JSVM_SUB)
				_JS_EXECUTE_BINARY(op_sub);
			else if (func == JSVM_HALT)
				pc = -1;
			else
				assert(0);
		}
	}
	return 0;
}