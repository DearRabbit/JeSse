#pragma once

#include <stdlib.h>

#include "object.h"

typedef struct {
	JsObject_HEAD
	long ob_ival;
} JsIntObject;

extern JsTypeObject JsInt_Type;

#define JsInt_CheckType(obj) ((obj)->ob_type == &JsInt_Type)

JsTypeObject* JsInt_FromLong(long);