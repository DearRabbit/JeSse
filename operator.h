#pragma once

#include "object.h"

JsObject* op_add(JsObject*, JsObject*);
JsObject* op_sub(JsObject*, JsObject*);
JsObject* op_mul(JsObject*, JsObject*);
JsObject* op_div(JsObject*, JsObject*);

JsObject* op_and(JsObject*, JsObject*);
JsObject* op_or(JsObject*, JsObject*);
JsObject* op_xor(JsObject*, JsObject*);