#include <stdio.h>
#include <stdlib.h>

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

	JsObject* a = JsString_FromString("hahahah");
	JsObject* b = JsString_FromString("hahahah");
	PRINTOBJ(a);

	printf("%d\n", _JsString_Eq(a, b));

	_JsBool_Deinit();

	return 0;
}