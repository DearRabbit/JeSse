#pragma once

#include "object.h"

typedef struct {
    JsObject_HEAD
    double ob_ival;
} JsNumObject;

extern JsTypeObject JsNum_Type;
extern int errnoInNum;

#define JsNum_CheckType(obj) (Js_Type(obj) == &JsNum_Type)
#define JsNum_CheckSubClass(obj) (Js_Type(obj)->tp_flags & JS_TPFLAGS_INT_SUBCLASS)

// C - API
JsObject* JsNum_FromDouble(double);
double JsNum_GetDouble(JsObject *);
int JsNum_GetInt(JsObject *);

// macro access for speed?
#define JsNum_AS_DOUBLE(obj) (((JsNumObject *)(obj))->ob_ival)

