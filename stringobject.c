#include "runtime.h"

#define CHAROBJECT_MAX 256

static JsStringObject *characters[CHAROBJECT_MAX];
static JsStringObject *nullstring;

JsObject* JsString_FromStringAndSize(const char *str, ssize_t len)
{
	return NULL;
}

JsObject* JsString_FromString(const char *str)
{
	return NULL;
}

char * JsString_GetCString(JsObject *obj)
{
	return NULL;
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

static long
string_hash(JsStringObject *a)
{
    register ssize_t len;
    register unsigned char *p;
    register long x;

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

	JS_TPFLAGS_DEFAULT | JS_TPFLAGS_BASETYPE |
		JS_TPFLAGS_INT_CAST,			/* weak cast to int */

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
	// nullstring
	v = malloc(sizeof(JsStringObject));
	w = malloc( (sizeof(JsStringObject)+1) * CHAROBJECT_MAX );//len=1 
	if (v == NULL || w == NULL)
	{
		dbgprint("Initialization of stringobject failed");
		return -1;
	}
	nullstring = v;
	// TODO - initialization!
	for (i = 0; i < CHAROBJECT_MAX; ++i)
	{
		characters[i] = w + i;
	}
	return 0;
}

void
_JsString_Deinit(void)
{
	if (nullstring != NULL)
		free(nullstring);
	if (characters[0] != NULL)
		free(characters[0]);
}