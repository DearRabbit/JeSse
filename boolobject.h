#pragma once

#include "object.h"

typedef struct {
	JsObject_HEAD
	int ob_ival;
} JsBoolObject;

extern JsTypeObject JsBool_Type;
extern JsBoolObject _Js_FalseStruct;
extern JsBoolObject _Js_TrueStruct;

#define Js_False ((JsObject *) &_Js_FalseStruct)
#define Js_True ((JsObject *) &_Js_TrueStruct)

#define JsBool_CheckType(obj) (Js_Type(obj) == &JsBool_Type)

JsObject* JsBool_FromInt(int);
int JsBool_GetBool(JsObject *);
