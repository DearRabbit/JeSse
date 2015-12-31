#include <stdio.h>

#include "runtime.h"

#ifdef JS_DEBUG
extern JsIntObject* small_ints[];
extern JsStringObject *characters[];
extern JsStringObject *nullstring;
#endif

int main()
{
	_JsString_Init();
	JsStringObject *v = JsString_FromString("123");
	int w = JsInt_GetLong(v);
	
	printf("%d\n", w );

	Js_DECREF(v);
	_JsString_Deinit();
	return 0;
}