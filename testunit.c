#include <stdio.h>

#include "runtime.h"

#ifdef JS_DEBUG
extern JsIntObject* small_ints[];
#endif

int main()
{
	_JsInt_Init();
	printf("Int Object size:%ld\n", sizeof(JsIntObject));

#ifdef JS_DEBUG
	JsTypeObject* inttype = Js_Type(small_ints[0]);
	printf("%ld\n", JsInt_GetLong(0)); // -7
	printf("%ld\n", (0xffffffffL + 1));

#endif
	
	_JsInt_Deinit();
	return 0;
}