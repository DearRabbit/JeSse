#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "runtime.h"

#define CHAROBJECT_MAX 255

// nullstring is the first element of characters
#ifdef JS_DEBUG
	JsStringObject *characters[CHAROBJECT_MAX + 1];
	JsStringObject *nullstring;
#else
	static JsStringObject *characters[CHAROBJECT_MAX + 1];
	static JsStringObject *nullstring;
#endif

JsObject* JsString_FromStringAndSize(const char *str, size_t size)
{
	register JsStringObject *op;
	if (size == 0 && (op = nullstring) != NULL) 
	{
		Js_INCREF(op);
		return (JsObject *)op;
	}
	if (size == 1 && str != NULL &&
        (op = characters[*str & CHAROBJECT_MAX]) != NULL)
	{
		Js_INCREF(op);
		return (JsObject *)op;
	}

	op = (JsStringObject *)malloc(sizeof(JsStringObject) + size);
	if (op == NULL)
	{
		return JsRtErr_NoMemory();
	}
	JsObject_INIT_VAR(op, &JsString_Type, size);
	op->ob_shash = -1;
	if (str != NULL)
		memcpy(op->ob_sval, str, size);
	op->ob_sval[size] = '\0';

	return (JsObject *) op;
}

JsObject* JsString_FromString(const char *str)
{
	return JsString_FromStringAndSize(str, strlen(str));
}

char * JsString_GetCString(JsObject *obj)
{
	JsStringObject *v;
	if (obj && JsString_CheckType(obj))
	{
		v = (JsStringObject *)obj;
		return v->ob_sval;
	}
	if (obj == NULL || !JsString_CheckCast(obj))
	{
		dbgprint("Invalid type in deallocation of stringobject\n");
		// TODO: ?return sth?
		return NULL;
	}
	// when to release?
	v = (JsStringObject *)(Js_Type(obj)->tp_tostr(obj));
	return v->ob_sval;
}


static void
string_dealloc(JsStringObject *obj)
{
	if (JsString_CheckType(obj))
		free(obj);
	else
		dbgprint("Invalid type '%s' in deallocation of stringobject\n", Js_Type(obj)->tp_name);
}

static int
string_print(JsStringObject *obj, FILE *fp)
{
	// unlike int, mismatch of type could cause alot problems.
	if (JsString_CheckType(obj))
	{
		return fprintf(fp, "%s\n", obj->ob_sval);
	}
	else
	{
		dbgprint("unable to print '%s'\n", Js_Type(obj)->tp_name);
		return -1;
	}
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
#if (2147483647L + 1L > 0)
	#define HashSecretPartA 0x16de24d2bf177e92L
	#define HashSecretPartB 0x3a7034f7c2d3d57cL
#else
	#define HashSecretPartA 0x16de24d2L
	#define HashSecretPartB 0x3a7034f7L
#endif

static u64
string_hash(JsStringObject *a)
{
    register ssize_t len;
    register unsigned char *p;
    register u64 x;

    if (a->ob_shash != -1)
        return a->ob_shash;
    len = Js_Size(a);
    /*
      We make the hash of the empty string be 0, rather than using
      (prefix ^ suffix), since this slightly obfuscates the hash secret
    */
    if (len == 0) {
        a->ob_shash = 0;
        return 0;
    }
    p = (unsigned char *) a->ob_sval;
    x = HashSecretPartA;
    x ^= *p << 7;
    while (--len >= 0)
        x = (1000003*x) ^ *p++;
    x ^= Js_Size(a);
    x ^= HashSecretPartB;
    if (x == -1)
        x = -2;
    a->ob_shash = x;
    return x;
}

/* JsStringObject size : any memory allocation for a string 
   of length n should request sizeof(JsStringObject) + n bytes.
*/
JsTypeObject JsString_Type = {
	JsObject_HEAD_INIT(&JsType_Type)
	"str",
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
	int i;
	JsStringObject *v, *w;
	size_t charPoolSize = (sizeof(JsStringObject)+1) * CHAROBJECT_MAX;//len=1 

	// nullstring
	v = malloc(charPoolSize);

	if (v == NULL)
	{
		dbgprint("Initialization of stringobject failed");
		return -1;
	}

	memset(v, 0, charPoolSize);

	// initialization of ""
	JsObject_INIT_VAR(v, &JsString_Type, 0);
	nullstring = v;

	// initialization of 
	for (i=1, w=v+1; i<=CHAROBJECT_MAX; ++i, ++w)
	{
		JsObject_INIT_VAR(w, &JsString_Type, 1);
		*(w->ob_sval) = i;
		characters[i] = w;
	}
	return 0;
}

void
_JsString_Deinit(void)
{
	if (nullstring != NULL)
		free(nullstring);
}