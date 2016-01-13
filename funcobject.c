#include "runtime.h"

JsObject*
JsDef_NewInstance(JsObject* def)
{
	register JsFuncObject* alloc;

	alloc = Js_Malloc(sizeof(JsFuncObject));

	JsObject_INIT(alloc, &JsFunc_Type);
	alloc->func_def = def;

	// var_table should be new in calling
	alloc->var_table = NULL;
	return alloc;
}

static void
def_dealloc(JsDefObject *obj)
{

}

static void
func_dealloc(JsFuncObject *obj)
{

}

static int
func_print(JsFuncObject *obj, FILE *fp)
{
	return fprintf(fp, "[Function]");
}

static JsObject *
def_tostring(JsDefObject *v) 
{
    return (JsObject*)(v->func_string);
}

static JsObject *
func_tostring(JsFuncObject *v) 
{
    return (JsObject*)(JsFunc_GetDef(v)->func_string);
}

JsTypeObject JsDef_Type =
{
	JsObject_HEAD_INIT(&JsType_Type)
	"function",
	sizeof(JsDefObject),
	0,

	JS_TPFLAGS_DEFAULT,

	NULL,							/* no new func, generate by api*/
	(destructor)def_dealloc,		/* tp_dealloc */
	(printfunc)func_print,			/* tp_print */
	(tostringfunc)def_tostring,		/* tp_tostr */

	NULL,							/* tp_compare */
	NULL,							/* tp_hash */
};

JsTypeObject JsFunc_Type =
{
	JsObject_HEAD_INIT(&JsType_Type)
	"function",
	sizeof(JsFuncObject),
	0,

	JS_TPFLAGS_DEFAULT,

	NULL,							/* no new func, generate by api*/
	(destructor)func_dealloc,		/* tp_dealloc */
	(printfunc)func_print,			/* tp_print */
	(tostringfunc)func_tostring,	/* tp_tostr */

	NULL,							/* tp_compare */
	NULL,							/* tp_hash */
};