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
	JsStringObject *v = JsString_FromString("nullstring");

	printf("%s\n", JsString_GetCString(characters['a']) );
	
	printf("%s\n", JsString_GetCString(v) );

	Js_DECREF(v);
	_JsString_Deinit();
	return 0;
}