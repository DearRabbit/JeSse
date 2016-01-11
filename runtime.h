#pragma once

#include "object.h"
#include "numobject.h"
#include "stringobject.h"
#include "boolobject.h"

#include "dictobject.h"

typedef struct _freelist{
	void *ptr;
	struct _freelist* next;
} JsFreeList;

typedef JsObject JsNullObject;
typedef JsObject JsUndefObject;

extern JsTypeObject JsNull_Type;
extern JsTypeObject JsUndef_Type;

extern JsNullObject _Js_NullStruct;
extern JsUndefObject _Js_UndefStruct;

#define _JsTempVarName_Max 8
extern JsObject* _Js_TempVarName[_JsTempVarName_Max];

#define JsException_Normal			0
#define JsException_ParseError		-1
#define JsException_NullPtrError	-2

// Initialization should be the same order as below.

int _JsNum_Init(void);
void _JsNum_Deinit(void);

int _JsString_Init(void);
void _JsString_Deinit(void);

int _JsBool_Init(void);
void _JsBool_Deinit(void);

int _JsDefaultVar_Init(void);
void _JsDefaultVar_Deinit(void);

// Exit on error, never return NULL
void* Js_Malloc(size_t);
void Js_Free(void*);
