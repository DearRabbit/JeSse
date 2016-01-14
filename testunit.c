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

	JsStringObject* var1 = JsString_FromString("var1");
	JsStringObject* var2 = JsString_FromString("var2");
	JsStringObject* var3 = JsString_FromString("var3");
	JsNumObject* value1 = JsNum_FromDouble(1);
	JsNumObject* value2 = JsNum_FromDouble(2);
	JsNumObject* value3 = JsNum_FromDouble(3);

	JsDictObject* mydict = JsDict_New();
	JsDict_SetItem(mydict, var1, value1);
	JsDict_SetItem(mydict, var2, value2);
	JsDict_SetItem(mydict, var3, value3);
	JsDict_DelItem(mydict, var3);

	JsObject* ptr = JsDict_GetItem(mydict, var3);
	

	// vmcode test[13] = {
	// 	JSVM_EQ,
	// 	_(one),
	// 	_(string_two),
	// 	JSVM_SetJump_Offset(-4),
	// 	JSVM_HALT
	// };

	// jsvm_execute(test);

	//PRINTOBJ(vmresult);

	// Js_DECREF(one);
	// Js_DECREF(string_two);

	_JsDict_Deinit();
	_JsBaseVar_Deinit();
	_JsBool_Deinit();
	_JsNum_Deinit();

	Js_Exit();

	return 0;
}