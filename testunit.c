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
	_JsNum_Init();
	_JsBool_Init();
	_JsBaseVar_Init();
	_JsDict_Init();

	int i = 0;
	JsObject * dict_test = JsDict_New();
	JsStringObject* myvalue = JsString_FromString("myvalue");

	for (i = 0; i < _JsTempVarName_Max; ++i)
	{
		JsDict_SetItem(dict_test, _Js_TempVarName[i], myvalue);
	}

	JsStringObject* test_value = JsDict_GetItem(dict_test, _Js_TempVarName[2]);

	PRINTOBJ(test_value);

	_JsDict_Deinit();
	_JsBaseVar_Deinit();
	_JsBool_Deinit();
	_JsNum_Deinit();

	Js_Exit();

	return 0;
}