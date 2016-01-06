#pragma once

#include "object.h"

typedef struct {
	JsObject_VAR_HEAD
	long ob_shash;
	char ob_sval[1];

    /* Invariants:
     *     ob_sval contains space for 'ob_size+1' elements.
     *     ob_sval[ob_size] == 0.
     *     ob_shash is the hash of the string or -1 if not computed yet.
     *     ob_sstate != 0 iff the string object is in stringobject.c's
     *       'interned' dictionary; in this case the two references
     *       from 'interned' to this object are *not counted* in ob_refcnt.
     */
} JsStringObject;

extern JsTypeObject JsString_Type;

#define JsString_CheckType(obj) ((obj)->ob_type == &JsString_Type)
// #define JsString_CheckCast(obj) (Js_Type(obj)->tp_tostr != NULL)

// C - API
// unsafe?
JsObject* JsString_FromStringAndSize(const char *, size_t);
JsObject* JsString_FromString(const char *);
char * JsString_GetCString(JsObject *);

// for speed
#define JsString_AS_CSTRING(op) (((JsStringObject *)(op))->ob_sval)