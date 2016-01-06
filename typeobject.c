#include "object.h"

static JsObject *
type_new(JsTypeObject *type, JsObject *args, JsObject *kwds)
{
	// No implementaion.
	return NULL;
}

static void
type_dealloc(JsTypeObject *type)
{
	// free something
}

JsTypeObject JsType_Type = 
{
	JsObject_HEAD_INIT(&JsType_Type)
	"type",
	sizeof(JsTypeObject),
	0,

	JS_TPFLAGS_DEFAULT,

	NULL,						/* tp_new */
	NULL,						/* tp_dealloc */
	NULL,						/* tp_print */
	NULL,						/* tp_tostr */

	NULL,						/* tp_compare */
	(hashfunc)_Js_HashPointer,	/* tp_hash */
};