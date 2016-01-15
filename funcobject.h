#pragma once

#include "object.h"

typedef struct _astStruct JsAst;
typedef struct _defobject JsDefObject;
typedef struct _funcobject JsFuncObject;
struct _defobject{
	JsObject_HEAD
	
	// int argv_count;
	/*	for function like
	 *	"function(){
	 *		return 0;
	 *	}"
	 *		
	 *	char* func_string = 
	 *	'function(){\n\treturn 0;\n}'
	 */
	// JsStringObject* func_string;

	/*	$global-def
	 *	 |
	 *	$global-instance
	 *	 |
	 *	func1-def - func1-instance
	 *
	 */
	JsFuncObject* scoping;
	// vmcode* vm_set;

	JsAst* ast;
};

extern JsTypeObject JsDef_Type;

#define JsDef_CheckType(obj) (Js_Type(obj) == &JsDef_Type)
JsObject* JsDef_NewInstance(JsObject* def);
#define JsDef_GetAst(obj) (((JsDefObject*)(obj)) -> ast)

struct _funcobject{
	JsObject_HEAD

	JsDefObject* func_def;

	JsFuncObject* callee_func;
	JsFuncObject* caller_func;

	JsDictObject* var_table;
};

extern JsTypeObject JsFunc_Type;

#define JsFunc_CheckType(obj) (Js_Type(obj) == &JsFunc_Type)

#define JsFunc_GetDef(obj) (((JsFuncObject*)(obj))->func_def)
#define JsFunc_GetScope(obj) (JsFunc_GetDef(obj)->scoping)
#define JsFunc_GetCode(obj) (JsFunc_GetDef(obj)->vm_set)

#define JsFunc_SetCallingRelation(caller, callee) do						\
	{																		\
		((JsFuncObject*)(caller))->callee_func = ((JsFuncObject*)(callee));	\
		((JsFuncObject*)(callee))->caller_func = ((JsFuncObject*)(caller));	\
	} while (0)
	
int JsFunc_DefVariable(JsFuncObject* func, JsObject* name, JsObject* value);
int JsFunc_SetVariable(JsFuncObject* func, JsObject* name, JsObject* value);
JsObject* JsFunc_GetVariable(JsFuncObject* func, JsObject* name);

