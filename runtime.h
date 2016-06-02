#pragma once

#include "object.h"
#include "numobject.h"
#include "stringobject.h"
#include "boolobject.h"

#include "dictobject.h"
#include "funcobject.h"

#include "operator.h"

typedef JsObject JsNullObject;
typedef JsObject JsUndefObject;

extern JsTypeObject JsNull_Type;
extern JsTypeObject JsUndef_Type;

extern JsNullObject _Js_NullStruct;
extern JsUndefObject _Js_UndefStruct;

#define Js_Null ((JsObject *) &_Js_NullStruct)
#define Js_Undefined ((JsObject *) &_Js_UndefStruct)

#define _JsTempVarName_Max 8
extern JsObject* _Js_TempVarName[_JsTempVarName_Max];

#define JsException_Normal          0
#define JsException_ParseError      -1
#define JsException_NullPtrError    -2

// Initialization should be the same order as below.

int _JsNum_Init(void);
void _JsNum_Deinit(void);

int _JsString_Init(void);
void _JsString_Deinit(void);

int _JsBool_Init(void);
void _JsBool_Deinit(void);

int _JsBaseVar_Init(void);
void _JsBaseVar_Deinit(void);

int _JsDict_Init(void);
void _JsDict_Deinit(void);

// Exit on error, never return NULL
void* Js_Malloc(size_t);
void Js_Free(void*);
void Js_Exit(void);