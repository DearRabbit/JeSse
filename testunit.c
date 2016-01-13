#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>

#include "runtime.h"

extern JsObject* vmresult;

#define PRINTOBJ(obj) (Js_Type(obj)->tp_print(obj, stdout))
#define HASHOBJ(obj) (Js_Type(obj)->tp_hash(obj))
#define TOSTRINGOBJ(obj) (Js_Type(obj)->tp_tostr(obj))

int main()
{
	_JsNum_Init();
	_JsBool_Init();
	_JsBaseVar_Init();
	_JsDict_Init();

	JsNumObject* one = JsNum_FromDouble(0);
	JsStringObject* string_two = JsString_FromString("1");

	vmcode test[13] = {
		JSVM_EQ,
		one,
		string_two,
		JSVM_EQ,
		string_two,
		one,
		JSVM_ADD,
		one,
		string_two,
		JSVM_EQ,
		string_two,
		one,
		JSVM_HALT
	};

	jsvm_execute(test);

	_JsDict_Deinit();
	_JsBaseVar_Deinit();
	_JsBool_Deinit();
	_JsNum_Deinit();

	Js_Exit();

	return 0;
}