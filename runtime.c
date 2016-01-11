#include "runtime.h"

#include <stdlib.h>

#define VAR_NULL_STRING "null"
#define VAR_UNDEF_STRING "undefined"

// Error On Exit, never return NULL
void*
Js_Malloc(size_t len)
{
	void *space = malloc(len);
	if (space == NULL)
	{
		Js_FatalError("Not enough memory");
	}
	return space;
}

void 
Js_Free(void* ptr)
{
	if (ptr != NULL)
		free(ptr);
}


// null & undefined

static JsObject *null_str;
static JsObject *undef_str;

int
_JsDefaultVar_Init(void)
{
	JsObject *v, *w;

	v = JsString_FromString(VAR_NULL_STRING);
	if (v == NULL)
	{
		dbgprint("Initialization of runtime environment failed");
		return -1;
	}
	w = JsString_FromString(VAR_UNDEF_STRING);
	if (w == NULL)
	{
		dbgprint("Initialization of runtime environment failed");
		return -1;
	}

	null_str = v;
	undef_str = w;

	return 0;
}

void
_JsDefaultVar_Deinit(void)
{
	if (null_str != NULL && undef_str != NULL)
	{
		free(null_str);
		free(undef_str);
	}
}

static int
null_print(JsObject *obj, FILE *fp)
{
	fprintf(fp, "%s", VAR_NULL_STRING);
	return 0;
}

static JsObject *
null_tostr(JsObject *obj)
{
	return null_str;
}

static int
undef_print(JsObject *obj, FILE *fp)
{
	fprintf(fp, "%s", VAR_UNDEF_STRING);
	return 0;
}

static JsObject *
undef_tostr(JsObject *obj)
{
	return undef_str;
}

JsTypeObject JsNull_Type = {
	JsObject_HEAD_INIT(&JsType_Type)
	VAR_NULL_STRING,
	sizeof(JsNullObject),
	0,

	JS_TPFLAGS_DEFAULT,

	NULL,							/* tp_new*/
	NULL,							/* tp_dealloc */
	(printfunc)null_print,			/* tp_print */
	(tostringfunc)null_tostr,		/* tp_tostr */

	NULL,							/* tp_compare */
	NULL,							/* tp_hash */
};

JsTypeObject JsUndef_Type = {
	JsObject_HEAD_INIT(&JsType_Type)
	VAR_UNDEF_STRING,
	sizeof(JsUndefObject),
	0,

	JS_TPFLAGS_DEFAULT,

	NULL,							/* tp_new*/
	NULL,							/* tp_dealloc */
	(printfunc)undef_print,			/* tp_print */
	(tostringfunc)undef_tostr,		/* tp_tostr */

	NULL,							/* tp_compare */
	NULL,							/* tp_hash */
};

JsNullObject _Js_NullStruct = {
	JsObject_HEAD_INIT(&JsNull_Type)
};

JsUndefObject _Js_UndefStruct = {
	JsObject_HEAD_INIT(&JsUndef_Type)
};