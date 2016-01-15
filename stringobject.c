#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "runtime.h"

#define CHAROBJECT_MAX 255

// use nullstring as constant pool
static JsStringObject _Js_Nullstring;

JsObject*
JsString_FromStringAndSize(const char *str, size_t size)
{
	register JsStringObject *op;
	if (size == 0 && (op = &_Js_Nullstring) != NULL) 
	{
		Js_INCREF(op);
		return (JsObject *)op;
	}

	op = (JsStringObject *)Js_Malloc(sizeof(JsStringObject) + size);
	JsObject_INIT_VAR(op, &JsString_Type, size);
	op->ob_shash = 0;
	if (str != NULL)
		memcpy(op->ob_sval, str, size);
	op->ob_sval[size] = '\0';

	return (JsObject *) op;
}

JsObject*
JsString_FromString(const char *str)
{
	return JsString_FromStringAndSize(str, strlen(str));
}

JsObject*
JsString_GetString(JsObject *obj)
{
	JsObject *v;
	if (obj && JsString_CheckType(obj))
	{
		return obj;
	}
	if (obj == NULL)
	{
		dbgprint("NullPointer in JsString_GetCString\n");
		return Js_Null;
	}
	// when to release?
	v = (Js_Type(obj)->tp_tostr(obj));
	return v;
}

char *
JsString_GetCString(JsObject *obj)
{
	JsObject *v = (JsObject*)JsString_GetString(obj);
	if (v != Js_Null)
		return JsString_AS_CSTRING(v);
	else
		return "null";
}

JsObject*
_JsString_StringJoin(JsObject *v, JsObject *w)
{
	char *a = JsString_AS_CSTRING(v);
	char *b = JsString_AS_CSTRING(w);

	size_t len_a = strlen(a);
	size_t size = len_a + strlen(b);

	JsStringObject *op;

	if (size == 0 && (op = &_Js_Nullstring) != NULL) 
	{
		Js_INCREF(op);
		return (JsObject *)op;
	}

	op = (JsStringObject *)Js_Malloc(sizeof(JsStringObject) + size);
	JsObject_INIT_VAR(op, &JsString_Type, size);
	op->ob_shash = 0;
	if (a != NULL && b != NULL)
	{
		memcpy(op->ob_sval, a, len_a+1);
		strcat(op->ob_sval, b);
	}
	op->ob_sval[size] = '\0';

	return (JsObject *) op;
}

int
_JsString_Eq(JsObject *o1, JsObject *o2)
{
	JsStringObject *a = (JsStringObject*) o1;
	JsStringObject *b = (JsStringObject*) o2;
	return Js_Size(a) == Js_Size(b)
      && memcmp(a->ob_sval, b->ob_sval, Js_Size(a)) == 0;
}

static void
string_dealloc(JsStringObject *obj)
{
	Js_Free(obj);
}

static int
string_print(JsStringObject *obj, FILE *fp)
{
	// unlike number, mismatch of type could cause alot problems.
	return fprintf(fp, "\'%s\'", obj->ob_sval);
}

static JsObject *
string_tostr(JsStringObject *v)
{
	return (JsObject *)v;
}

static int
string_compare(JsStringObject *v, JsStringObject *w)
{
	//if (JsString_CheckType(v) && JsString_CheckType(w))
		return strcmp(v->ob_sval, w->ob_sval);
	//else //TODO: maybe add some casting
	//	dbgprint("unable to print '%s'\n", Js_Type(obj)->tp_name);
}

/* MD5 (LICENSE) = 16de24d23a7034f7bf177e92c2d3d57c */
//#if (2147483647L + 1L > 0)
#ifdef HASH_AS_LONG
	#define HashSecretPartA 0x16de24d2bf177e92L
	#define HashSecretPartB 0x3a7034f7c2d3d57cL
#else
	#define HashSecretPartA 0x16de24d2L
	#define HashSecretPartB 0x3a7034f7L
#endif

static uhash
string_hash(JsStringObject *a)
{
    register ssize_t len;
    register unsigned char *p;
    register uhash x = 0;

    if (a->ob_shash != 0)
        return a->ob_shash;
    len = Js_Size(a);

    if (len == 0) {
        a->ob_shash = 0;
        return 0;
    }
    p = (unsigned char *) a->ob_sval;

    /*
    	Like Java, the hashcode is:
		s[0]*31^(n-1) + s[1]*31^(n-2) + ... + s[n-1]
    */
    while (--len >= 0)
        x = (31*x) + *(p++);

    a->ob_shash = x;
    return x;
}

/* JsStringObject size : any memory allocation for a string 
   of length n should request sizeof(JsStringObject) + n bytes.
*/
JsTypeObject JsString_Type = {
	JsObject_HEAD_INIT(&JsType_Type)
	"string",
	sizeof(JsStringObject),
	sizeof(char),

	JS_TPFLAGS_DEFAULT | JS_TPFLAGS_BASETYPE ,

	NULL,								/* no new func, generate by api*/
	(destructor)string_dealloc,			/* tp_dealloc */
	(printfunc)string_print,			/* tp_print */
	(tostringfunc)string_tostr,			/* tp_tostr */

	(cmpfunc)string_compare,			/* tp_compare */
	(hashfunc)string_hash,				/* tp_hash */
};

int
_JsString_Init(void)
{
	// JsStringObject *v;
	// v = malloc(sizeof(JsStringObject));

	// if (v == NULL)
	// {
	// 	dbgprint("Initialization of stringobject failed");
	// 	return -1;
	// }

	// // initialization of nullstring
	// JsObject_INIT_VAR(v, &JsString_Type, 0);
	// *(v->ob_sval) = 0;
	// nullstring = v;

	return 0;
}

void
_JsString_Deinit(void)
{
	// if (nullstring != NULL)
	// 	free(nullstring);
}

static JsStringObject _Js_Nullstring = {
	JsVarObject_HEAD_INIT(&JsString_Type, 0)
	0,
	{0}
};