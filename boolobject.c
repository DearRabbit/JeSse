#include <stdlib.h>

#include "runtime.h"

static JsObject *true_str;
static JsObject *false_str;

JsObject*
JsBool_FromInt(int val)
{
	JsObject* ret;
	if (val)
		ret = Js_True;
	else
		ret = Js_False;

	Js_INCREF(ret);
	return ret;
}

int
JsNum_GetBool(JsObject *obj)
{
	return ((JsBoolObject*)obj)->ob_ival;
}

static int
bool_print(JsBoolObject *obj, FILE *fp)
{
	fputs(obj->ob_ival == 0 ? "False" : "True", fp);
	return 0;
}

static JsObject *
bool_tostring(JsBoolObject *v)
{
	JsObject* ret;
	if (v->ob_ival)
		ret = true_str;
	else
		ret = false_str;

	Js_INCREF(ret);
	return ret;
}

JsTypeObject JsBool_Type =
{
	JsObject_HEAD_INIT(&JsType_Type)
	"boolean",
	sizeof(JsBoolObject),
	0,

	JS_TPFLAGS_DEFAULT | JS_TPFLAGS_BASETYPE,

	NULL,								/* no new func, generate by api*/
	NULL,								/* tp_dealloc */
	(printfunc)bool_print,				/* tp_print */
	(tostringfunc)bool_tostring,		/* tp_tostr */

	NULL,								/* tp_compare */
	NULL,								/* tp_hash */
};

JsBoolObject _Js_FalseStruct = {
    JsObject_HEAD_INIT(&JsBool_Type)
    0
};

JsBoolObject _Js_TrueStruct = {
    JsObject_HEAD_INIT(&JsBool_Type)
    1
};