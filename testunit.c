#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>

#include "jsvm.h"

#ifdef JS_DEBUG
extern JsStringObject *nullstring;
extern JsObject* vmresult;
#endif

#define PRINTOBJ(obj) (Js_Type(obj)->tp_print(obj, stdout))
#define HASHOBJ(obj) (Js_Type(obj)->tp_hash(obj))
#define TOSTRINGOBJ(obj) (Js_Type(obj)->tp_tostr(obj))

int main()
{
	_JsNum_Init();
	_JsBool_Init();
	_JsBaseVar_Init();
	_JsDict_Init();

	// JsNumObject* one = JsNum_FromDouble(-0);
	// JsStringObject* string_two = JsString_FromString("2");
	// vmcode testfunc[4] = {JSVM_SUB, one, string_two, JSVM_HALT};

	// jsvm_execute(testfunc);

	// assert(vmresult != NULL);
	// PRINTOBJ(vmresult);
	// PRINTOBJ(one);

	_JsDict_Deinit();
	_JsBaseVar_Deinit();
	_JsBool_Deinit();
	_JsNum_Deinit();

	Js_Exit();

	return 0;
}