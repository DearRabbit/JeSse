#include "runtime.h"

JsObject *
JsRtErr_NoMemory(void)
{
	dbgprint("Not enough memory");
	return NULL;
}