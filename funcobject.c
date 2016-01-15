#include "runtime.h"

#include <assert.h>

JsObject*
JsDef_New(JsFuncObject* scoping_val, JsAst* ast_val)
{
	register JsDefObject* alloc;

	assert(JsFunc_CheckType(scoping_val));
	alloc = Js_Malloc(sizeof(JsDefObject));

	JsObject_INIT(alloc, &JsDef_Type);

	alloc->scoping = scoping_val;
	alloc->ast = ast_val;

	Js_INCREF(scoping_val);
	return (JsObject*)alloc;
}

JsObject*
JsDef_NewInstance(JsObject* def)
{
	register JsFuncObject* alloc;

	assert(JsDef_CheckType(def));
	alloc = Js_Malloc(sizeof(JsFuncObject));

	JsObject_INIT(alloc, &JsFunc_Type);
	alloc->func_def = (JsDefObject*)def;
	Js_INCREF(def);

	// var_table should be new in calling_func
	alloc->var_table = (JsDictObject*)JsDict_New();
	return (JsObject*)alloc;
}

/* return the exact function vartable of given var name;
 * search the local variable first.
 * do not support using before declaration.
 */
#define _JsFunc_VarTable(func) (((JsFuncObject*)(func))->var_table)

static JsDictObject*
_JsFunc_SearchVar(JsFuncObject* func, JsObject* name)
{
	JsObject* value;
	JsFuncObject* current_scope = func;
	JsDictObject* current_table;

	assert(func != NULL);
	while (current_scope)
	{
		current_table = _JsFunc_VarTable(current_scope);
		value = JsDict_GetItem((JsObject*)current_table, name);
		if (value)
			return current_table;
        if (current_scope->func_def == NULL)
            return NULL;
		current_scope = JsFunc_GetScope(current_scope);
	}
	return NULL;
}

int
JsFunc_DefVariable(JsFuncObject* func, JsObject* name, JsObject* value)
{
	return JsDict_SetItem((JsObject*)_JsFunc_VarTable(func), name, value);
}

int
JsFunc_SetVariable(JsFuncObject* func, JsObject* name, JsObject* value)
{
	JsDictObject* dict;
	dict = _JsFunc_SearchVar(func, name);
	if (dict == NULL)
	{
		return JsDict_SetItem((JsObject*)_JsFunc_VarTable(func),
			name, value);
	}
	else
	{
		return JsDict_SetItem((JsObject*)dict, name, value);
	}
}

JsObject*
JsFunc_GetVariable(JsFuncObject* func, JsObject* name)
{
	JsDictObject* dict;
	dict = _JsFunc_SearchVar(func, name);
	if (dict == NULL)
	{
		Js_FatalError("ReferenceError: undefined var name");
	}
	else
	{
		return JsDict_GetItem((JsObject*)dict, name);
	}
}

static void
def_dealloc(JsDefObject *obj)
{
	Js_XDECREF(obj->scoping);
	Js_Free(obj);
}

static void
func_dealloc(JsFuncObject *obj)
{
	Js_XDECREF(obj->func_def);
	Js_DECREF(obj->var_table);
	Js_Free(obj);
}

static int
def_print(JsDefObject *obj, FILE *fp)
{
	fprintf(fp, "[Function Definition]");
	return 0;
}

static int
func_print(JsFuncObject *obj, FILE *fp)
{
	fprintf(fp, "[Function]:{[var table]:");
	_Js_PRINTFP(obj->var_table, fp);
	fputc('}', fp);
	return 0;
}

static JsObject *
def_tostring(JsDefObject *v)
{
	// return (JsObject*)(v->func_string);
	JsObject *string = JsString_FromString("[Function Definition]");
	return string;
}

static JsObject *
func_tostring(JsFuncObject *v)
{
	// return (JsObject*)(JsFunc_GetDef(v)->func_string);
	JsObject *string = JsString_FromString("[Function]");
	return string;
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
	(printfunc)def_print,			/* tp_print */
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
