#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>

#include "runtime.h"

extern JsObject* vmresult;

#define PRINTOBJ(obj) (Js_Type(obj)->tp_print(obj, stdout))
#define HASHOBJ(obj) (Js_Type(obj)->tp_hash(obj))
#define TOSTRINGOBJ(obj) (Js_Type(obj)->tp_tostr(obj))

#define _(ob) ((vmcode)(ob))

int main()
{
	_JsNum_Init();
	_JsBool_Init();
	_JsBaseVar_Init();
	_JsDict_Init();

	JsNumObject* one = JsNum_FromDouble(2);
	JsStringObject* string_two = JsString_FromString("2");

	vmcode test[13] = {
		JSVM_EQ,
		_(one),
		_(string_two),
		JSVM_EQ,
		_(string_two),
		_(one),
		JSVM_ADD,
		_(one),
		_(string_two),
		JSVM_EQ,
		_(string_two),
		_(one),
		JSVM_HALT
	};

	jsvm_execute(test);

	PRINTOBJ(vmresult);

	Js_DECREF(one);
	Js_DECREF(string_two);

	_JsDict_Deinit();
	_JsBaseVar_Deinit();
	_JsBool_Deinit();
	_JsNum_Deinit();

	Js_Exit();

	return 0;
}