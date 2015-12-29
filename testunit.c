#include <stdio.h>

#include "intobject.h"
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
	printf("%ld\n", small_ints[0]->ob_ival); // -7
	inttype->tp_dealloc(inttype);

#endif
	
	_JsInt_Deinit();
	return 0;
}