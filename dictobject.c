#include "runtime.h"

static JsDictEntry
lookdict_string(JsDictObject *mp, JsObject *key, uhash hash);

static JsObject *
dict_new(JsTypeObject *type, JsObject *args, JsObject *kwds)
{
	return NULL;
}

static void
dict_dealloc(JsDictObject* obj)
{
	if (obj->ma_table != obj->ma_smalltable)
	{
		Js_Free(obj->ma_table);
	}
	Js_Free(obj);
}

JsTypeObject JsDict_Type = {
	JsObject_HEAD_INIT(&JsType_Type)
	"object",			// in Javascript, it's object instead of dict
	sizeof(JsDictObject),
	0,

	JS_TPFLAGS_DEFAULT, // not a basetype

	(newfunc)dict_new,					/* tp_new, only way to generate.*/
	(destructor)dict_dealloc,			/* tp_dealloc */
	//(printfunc)dict_print,				/* tp_print */
	//(tostringfunc)dict_tostring,		/* tp_tostr */
	NULL,
	NULL,

	NULL,								/* tp_compare */
	(hashfunc)_Js_HashPointer,			/* tp_hash, use the common version */
};

