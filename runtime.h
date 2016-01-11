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

#define JsException_Normal			0
#define JsException_ParseError		-1
#define JsException_NullPtrError	-2

int _JsNum_Init(void);
void _JsNum_Deinit(void);

int _JsString_Init(void);
void _JsString_Deinit(void);

int _JsBool_Init(void);
void _JsBool_Deinit(void);

// Exit on error, never return NULL
void* Js_Malloc(size_t);
void Js_Free(void*);
