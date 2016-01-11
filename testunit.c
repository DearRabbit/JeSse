#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "runtime.h"

#ifdef JS_DEBUG
extern JsStringObject *nullstring;
#endif

#define PRINTOBJ(obj) (Js_Type(obj)->tp_print(obj, stdout))
#define HASHOBJ(obj) (Js_Type(obj)->tp_hash(obj))
#define TOSTRINGOBJ(obj) (Js_Type(obj)->tp_tostr(obj))

int main()
{
	_JsBool_Init();

	JsObject* a = JsString_FromString("dsa");
	JsObject* b = JsString_FromString("jsjs");

	JsObject* c = op_add(a, b);
	PRINTOBJ(c);

	_JsBool_Deinit();

	return 0;
}