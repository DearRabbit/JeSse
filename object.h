#pragma once

#include <stdio.h>
#include "utils.h"

#define JsObject_HEAD				\
	ssize_t ob_refcnt;				\
	struct _typeobject *ob_type;

#define JsObject_VAR_HEAD			\
    JsObject_HEAD					\
    ssize_t ob_size; /* Number of items in variable part */

#define JsObject_HEAD_INIT(type)  1, type,

#define JsVarObject_HEAD_INIT(type, size)	\
    JsObject_HEAD_INIT(type) size,

#define INVALID_SIZE (ssize_t)-1

// Object - define:
// Object, VarObject
typedef struct _object {
	JsObject_HEAD
} JsObject;

typedef struct {
	JsObject_VAR_HEAD
} JsVarObject;

// Object - basic methods
#define Js_RefCnt(obj)	(((JsObject*)(obj)) -> ob_refcnt)
#define Js_Type(obj)	(((JsObject*)(obj)) -> ob_type)
#define Js_Size(obj)	(((JsVarObject*)(obj)) -> ob_size)

#define _Js_NewReference(obj)						\
    (Js_RefCnt(obj) = 1)
#define _Js_Dealloc(obj)							\
    ((*Js_Type(obj)->tp_dealloc)((JsObject *)(obj)))

#define JsObject_INIT(obj, typeobj) \
    ( Js_Type(obj) = (typeobj), _Js_NewReference((JsObject *)(obj)), (obj) )
#define JsObject_INIT_VAR(obj, typeobj, size) \
    ( Js_Size(obj) = (size), JsObject_INIT((obj), (typeobj)) )

#define Js_INCREF(obj)								\
    (((JsObject*)(obj))->ob_refcnt++)
#define Js_DECREF(obj)								\
	do {											\
		if (--((JsObject*)(obj))->ob_refcnt == 0)	\
			_Js_Dealloc(obj);						\
	}while(0)

// typedef - function ptr groups:
// operators
typedef JsObject* (*unaryfunc)(JsObject *);
typedef JsObject* (*binaryfunc)(JsObject *, JsObject *);
typedef JsObject* (*ternaryfunc)(JsObject *, JsObject *, JsObject *);
typedef int (*inquiry)(JsObject *);
typedef ssize_t (*lenfunc)(JsObject *);
// methods
// newfunc: TypeObject *type, Object *args, Object *kwds
// args stores arguments without key, kwds is a list
typedef JsObject *(*newfunc)(struct _typeobject *type, JsObject *args, JsObject *kwds);
typedef void (*destructor)(JsObject *obj);
// print to FILE* fp
typedef int (*printfunc)(JsObject *obj, FILE *fp);
typedef JsObject* (*tostringfunc)(JsObject *obj);
// return neg if i<j, pos if i>j, 0 if equals
typedef int (*cmpfunc)(JsObject *obja, JsObject *objb);
typedef uhash (*hashfunc)(JsObject *obj);

// TODO: Type Flags List: add more?
#define JS_TPFLAGS_DEFAULT			(1L<<0)
#define JS_TPFLAGS_BASETYPE			(1L<<10)

// #define JS_TPFLAGS_INT_CAST			(1L<<23)

// Object - define:
// TypeObject
typedef struct _typeobject {
	JsObject_HEAD
	const char *tp_name; /* For printing */
	ssize_t tp_basicsize, tp_itemsize; /* For allocation */

	/* Flags to define presence of optional/expanded features */
    long tp_flags;

	/* Methods to implement standard operations */
	newfunc tp_new;
	destructor tp_dealloc;
	printfunc tp_print;
	tostringfunc tp_tostr;

	cmpfunc tp_compare;
	hashfunc tp_hash;

	/* user-defined class def*/
	//struct JsMethodDef *tp_methods;
	//struct JSMemberDef *tp_members;
} JsTypeObject;

// basic typeobj
extern JsTypeObject JsType_Type;
// extern JsTypeObject JsBaseObject_Type; 

// common hash function
uhash _Js_HashPointer(void*);

#define JsType_CheckType(obj) ((obj)->ob_type == &JsTypeObject)

