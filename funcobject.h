#pragma once

#include "object.h"

typedef struct _defobject JsDefObject;
typedef struct _funcobject JsFuncObject;
struct _defobject{
	JsObject_HEAD
	
	/*	for function like
	 *	"function(){
	 *		return 0;
	 *	}"
	 *		
	 *	char* func_string = 
	 *	'function(){\n\treturn 0;\n}'
	 */
	JsStringObject* func_string;

	/*	$global-def
	 *	 |
	 *	$global-instance
	 *	 |
	 *	func1-def - func1-instance
	 *
	 */
	JsFuncObject* scoping;
	vmcode* vm_set;
};

extern JsTypeObject JsDef_Type;

#define JsDef_CheckType(obj) (Js_Type(obj) == &JsDef_Type)


struct _funcobject{
	JsObject_HEAD
	
	JsDefObject* func_def;
	JsDictObject* var_table;
};

extern JsTypeObject JsFunc_Type;

#define JsFunc_CheckType(obj) (Js_Type(obj) == &JsFunc_Type)

#define JsFunc_GetDef(obj) (((JsFuncObject*)(obj))->func_def)