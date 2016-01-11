#include "runtime.h"

#include <stdlib.h>

#define VAR_NULL_STRING "null"
#define VAR_UNDEF_STRING "undefined"

JsObject* _Js_TempVarName[_JsTempVarName_Max];

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
	int i;
	char tmp_varName[3] = {0};

	v = JsString_FromString(VAR_NULL_STRING);
	w = JsString_FromString(VAR_UNDEF_STRING);

	null_str = v;
	undef_str = w;

	// for 0 ~ _JsTempVarName_Max, naming '$0'-'$x'
	tmp_varName[0] = '$';
	for (i = 0; i < _JsTempVarName_Max; ++i)
	{
		tmp_varName[1] = '0' + i;
		_Js_TempVarName[i] = JsString_FromString(tmp_varName);
	}

	return 0;
}

void
_JsDefaultVar_Deinit(void)
{
	int i;
	if (null_str != NULL)
		free(null_str);
	if (undef_str != NULL)
		free(undef_str);

	for (i = 0; i < _JsTempVarName_Max; ++i)
	{
		if (_Js_TempVarName[i] != NULL)
			free(_Js_TempVarName[i]);
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