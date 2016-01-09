#pragma once

#include "object.h"

typedef struct {
	JsObject_HEAD
	double ob_ival;
} JsNumObject;

extern JsTypeObject JsNum_Type;
extern int errnoInNum;

#define JsNum_CheckType(obj) ((obj)->ob_type == &JsNum_Type)

// #define JsNum_SetInt(obj) (((JsNumObject *)(obj))->int_flag = 1)
// #define JsNum_SetFloat(obj) (((JsNumObject *)(obj))->int_flag = 0)
// #define JsNum_GetIntFlag(obj) (((JsNumObject *)(obj))->int_flag)

// C - API
JsObject* JsNum_FromDouble(double);
double JsNum_GetDouble(JsObject *);

// macro access for speed?
#define JsNum_AS_DOUBLE(obj) (((JsNumObject *)(obj))->ob_ival)

