#pragma once

#include <stdlib.h>

#include "object.h"

typedef struct {
	JsObject_HEAD
	long ob_ival;
} JsIntObject;

extern JsTypeObject JsInt_Type;

#define JsInt_CheckType(obj) ((obj)->ob_type == &JsInt_Type)
#define JsInt_CheckCast(obj) (Js_Type(obj)->tp_flags&JS_TPFLAGS_INT_CAST)

// C - API
JsObject* JsInt_FromLong(long);
long JsInt_GetLong(JsObject *);

// macro access for speed?
#define JsInt_AS_LONG(obj) (((JsIntObject *)(obj))->ob_ival)