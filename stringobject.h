#pragma once

#include "object.h"

typedef struct {
	JsObject_VAR_HEAD
	int ob_shash;
	char ob_sval[1];

    /* Invariants:
     *     ob_sval contains space for 'ob_size+1' elements.
     *     ob_sval[ob_size] == 0.
     *     ob_shash is the hash of the string or 0 if not computed yet.
     *     ob_sstate != 0 iff the string object is in stringobject.c's
     *       'interned' dictionary; in this case the two references
     *       from 'interned' to this object are *not counted* in ob_refcnt.
     */
} JsStringObject;

extern JsTypeObject JsString_Type;

#define JsString_CheckType(obj) ((obj)->ob_type == &JsString_Type)

// C - API
// unsafe?
JsObject* JsString_FromStringAndSize(const char *, size_t);
JsObject* JsString_FromString(const char *);
JsObject* JsString_GetString(JsObject *);
char * JsString_GetCString(JsObject *);

JsObject* _JsString_StringJoin(JsObject *, JsObject *);

// use in dict for speed
int _JsString_Eq(JsObject *, JsObject *);

// for speed
#define JsString_AS_CSTRING(op) (((JsStringObject *)(op))->ob_sval)