#pragma once

#define JsObject_HEAD				\
	size_t ob_refcnt;				\
	struct _typeobject *ob_type;

#define JsObject_VAR_HEAD			\
    JsObject_HEAD					\
    size_t ob_size; /* Number of items in variable part */

#define JsObject_HEAD_INIT(type)  1, type,

#define JsVarObject_HEAD_INIT(type, size)	\
    JsObject_HEAD_INIT(type) size,

#define INVALID_SIZE (size_t)-1


// Object - define:
// Object, VarObject
typedef struct _object {
	JsObject_HEAD
} JsObject;

typedef struct {
	JsObject_VAR_HEAD
} JsVarObject;

// Object - basic methods
#define Js_GetRefCnt(obj)	(((JsObject*)(obj)) -> ob_refcnt)
#define Js_GetType(obj)		(((JsObject*)(obj)) -> ob_type)
#define Js_GetSize(obj)		(((JsObject*)(obj)) -> ob_size)

// typedef - function ptr groups:
// operators
typedef JsObject* (*unaryfunc)(JsObject *);
typedef JsObject* (*binaryfunc)(JsObject *, JsObject *);
typedef JsObject* (*ternaryfunc)(JsObject *, JsObject *, JsObject *);
typedef int (*inquiry)(JsObject *);
typedef size_t (*lenfunc)(JsObject *);
// methods
// newfunc: TypeObject *type, Object *args, Object *kwds
// args stores arguments without key, kwds is a list
typedef JsObject *(*newfunc)(JsTypeObject *, JsObject *, JsObject *);
typedef void (*destructor)(JsObject *);
// print to FILE* fp
typedef int (*printfunc)(JsObject *, FILE *);
typedef JsObject* (*tostringfunc)(JsObject *);
// return -1 if i<j, 1 if i>j, 0 if equals
typedef int (*cmpfunc)(JsObject *, JsObject *);
typedef long (*hashfunc)(JsObject *); 

// Object - define:
// TypeObject
typedef struct _typeobject {
	JsObject_VAR_HEAD
	const char *tp_name; /* For printing */
	size_t tp_basicsize, tp_itemsize; /* For allocation */

	/* Flags to define presence of optional/expanded features */
    long tp_flags;

	/* Methods to implement standard operations */
	newfunc tp_new;
	destructor tp_dealloc;
	printfunc tp_print;
	cmpfunc tp_compare;
	tostringfunc tp_str;
	hashfunc tp_hash;
	
	/* user-defined class def*/
	//struct JsMethodDef *tp_methods;
	//struct JSMemberDef *tp_members;
} JsTypeObject;



