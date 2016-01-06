#include <stdlib.h>

#include "runtime.h"

#ifndef NSMALLPOSINTS
#define NSMALLPOSINTS           0//257
#endif
#ifndef NSMALLNEGINTS
#define NSMALLNEGINTS           0//7
#endif
#if NSMALLNEGINTS + NSMALLPOSINTS > 0
/* References to small integers are saved in this array so that they
   can be shared.
   The integers that are saved are those in the range
   -NSMALLNEGINTS (inclusive) to NSMALLPOSINTS (not inclusive).

*/	
	static JsNumObject *small_ints[NSMALLNEGINTS + NSMALLPOSINTS];
#endif

#define NUM_BUFFERSIZE 24

// C - API
/**
	It has been fully tested on Mac OS X, clang;

	On most modern desktop Intel CPU, SSE instruction set is enabled.
	Conversion may use SSE like cvttsd2si, or cvtsi2sdq; but it won't 
	act like 'node.js(v8)'.
	PS:v8 uses Grisu?
*/
JsObject *
JsNum_FromDouble(double dval)
{
	register JsNumObject *v;
	long ival = dval;

#if NSMALLNEGINTS + NSMALLPOSINTS > 0
	if (-NSMALLNEGINTS <= ival && ival < NSMALLPOSINTS) {
		v = small_ints[ival + NSMALLNEGINTS];
		Js_INCREF(v);
		return (JsObject *) v;
	}
#endif

	v = malloc(sizeof(JsNumObject));
	if (v == NULL)
	{
		return JsRtErr_NoMemory();
	}
	JsObject_INIT(v, &JsNum_Type);
	v->ob_ival = dval;
	return (JsObject *) v;
}

double
JsNum_GetDouble(JsObject *obj)
{
	/*
	if (obj && JsInt_CheckType(obj))
	{
		return ((JsNumObject *)obj)->ob_ival;
	}
	if (obj == NULL || !JsInt_CheckCast(obj))
	{
		dbgprint("Invalid type in deallocation of intobject\n");
		return -1;
	}
	if (JsString_CheckType(obj))
	{
		char *endptr;
		long ret = strtol(JsString_AS_CSTRING(obj), &endptr, 0);
		if (*endptr != 0)
		{
			errno_int = -1;
			return -1;
		}
		return ret;
	}
	return -1;
	*/
}

// methods

static void
num_dealloc(JsNumObject *obj)
{
	if (JsNum_CheckType(obj))
		free(obj);
	else
		dbgprint("Invalid type in deallocation of numobject\n");
}

static void
_num_internal_tocstring(JsNumObject *v, char* buffer)
{
	double fval = v->ob_ival;
	long intval;

	if (fval == intval)
	{

	}
	else
	{

	}
}

static int
num_print(JsNumObject *v, FILE *fp)
{
	char buf[NUM_BUFFERSIZE] = {0};
	_num_internal_tocstring(v, buf);
	fprintf(fp, "%s", buf);
	return 0;
}

static JsObject *
num_tostring(JsNumObject *v) 
{
    char buf[NUM_BUFFERSIZE] = {0};
    _num_internal_tocstring(v, buf);
    return JsString_FromString(buf);
}

static int
num_compare(JsNumObject *v, JsNumObject *w)
{
	double i = v->ob_ival;
	double j = w->ob_ival;
	return (i < j) ? -1 : (i > j) ? 1 : 0;
}

static u64
num_hash(JsNumObject *v)
{
	u64* xptr = &(v->ob_ival);
	return *xptr;
}

int errnoInNum;

JsTypeObject JsNum_Type =
{
	JsObject_HEAD_INIT(&JsType_Type)
	"number",
	sizeof(JsNumObject),
	0,

	JS_TPFLAGS_DEFAULT | JS_TPFLAGS_BASETYPE,

	NULL,								/* no new func, generate by api*/
	(destructor)num_dealloc,			/* tp_dealloc */
	(printfunc)num_print,				/* tp_print */
	(tostringfunc)num_tostring,			/* tp_tostr */

	(cmpfunc)num_compare,				/* tp_compare */
	(hashfunc)num_hash,					/* tp_hash */
};

// return -1 if error
int
_JsNum_Init(void)
{
	JsNumObject *v;
	int ival;
#if NSMALLNEGINTS + NSMALLPOSINTS > 0
	v = malloc(sizeof(JsNumObject)*(NSMALLNEGINTS + NSMALLPOSINTS));
	if (v == NULL)
	{
		dbgprint("Initialization of numobject failed");
		return -1;
    }
    for (ival = -NSMALLNEGINTS; ival < NSMALLPOSINTS; ival++, v++) {
		JsObject_INIT(v, &JsNum_Type);
		JsNum_SetInt(v);
		v->ob_ival = ival;
		small_ints[ival + NSMALLNEGINTS] = v;
	}
#endif
	return 0;
}

void
_JsNum_Deinit(void)
{
#if NSMALLNEGINTS + NSMALLPOSINTS > 0
	if (small_ints[0]!=NULL)
		free(small_ints[0]);
#endif
}
