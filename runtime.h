#pragma once

#include "object.h"
#include "numobject.h"
#include "stringobject.h"

int _JsNum_Init(void);
void _JsNum_Deinit(void);

int _JsString_Init(void);
void _JsString_Deinit(void);

JsObject * JsRtErr_NoMemory(void);