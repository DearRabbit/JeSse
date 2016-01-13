#include "runtime.h"

#include <math.h>

JsObject*
op_add(JsObject* v, JsObject* w)
{
	if (JsNum_CheckType(v) && JsNum_CheckType(w))
	{
		// for faster access...
		double op1 = JsNum_AS_DOUBLE(v);
		double op2 = JsNum_AS_DOUBLE(w);
		double result = op1 + op2;

		return JsNum_FromDouble(result);
	}
	else
	{
		JsObject* str1 = JsString_GetString(v);
		JsObject* str2 = JsString_GetString(w);
		JsObject* strres = _JsString_StringJoin(v, w);

		return strres;
	}
}

// -, *, / are the same.
JsObject*
op_sub(JsObject* v, JsObject* w)
{
	double op1 = JsNum_GetDouble(v);
	double op2 = JsNum_GetDouble(w);
	double result = op1 - op2;

	return JsNum_FromDouble(result);
}

JsObject*
op_mul(JsObject* v, JsObject* w)
{
	double op1 = JsNum_GetDouble(v);
	double op2 = JsNum_GetDouble(w);
	double result = op1 * op2;

	return JsNum_FromDouble(result);
}

JsObject*
op_div(JsObject* v, JsObject* w)
{
	double op1 = JsNum_GetDouble(v);
	double op2 = JsNum_GetDouble(w);
	double result = op1 / op2;

	return JsNum_FromDouble(result);
}

// use int for logic operation
JsObject* 
op_and(JsObject* v, JsObject* w)
{
	int op1 = JsNum_GetInt(v);
	int op2 = JsNum_GetInt(w);
	int result = op1 & op2;

	return JsNum_FromDouble((double)result);
}

JsObject* 
op_or(JsObject* v, JsObject* w)
{
	int op1 = JsNum_GetInt(v);
	int op2 = JsNum_GetInt(w);
	int result = op1 | op2;

	return JsNum_FromDouble((double)result);
}

JsObject* 
op_xor(JsObject* v, JsObject* w)
{
	int op1 = JsNum_GetInt(v);
	int op2 = JsNum_GetInt(w);
	int result = op1 ^ op2;

	return JsNum_FromDouble((double)result);
}

JsObject* op_eq(JsObject* v, JsObject* w)
{
	JsTypeObject* vtype = Js_Type(v);
	JsTypeObject* wtype = Js_Type(w);

	if (vtype->tp_compare == NULL || wtype->tp_compare == NULL)
	{
		return (v==w) ? Js_True : Js_False;
	}
	if (vtype == wtype)
	{
		return (vtype->tp_compare(v, w) == 0) ? Js_True : Js_False;
	}

	double vdouble = JsNum_GetDouble(v);
	double wdouble = JsNum_GetDouble(w);

	return (vdouble==wdouble) ? Js_True : Js_False;
}
JsObject* op_neq(JsObject* v, JsObject* w)
{
	if (op_eq(v, w) == Js_False)
		return Js_True;
	return Js_False;
}
JsObject* op_lt(JsObject* v, JsObject* w)
{
	JsTypeObject* vtype = Js_Type(v);
	JsTypeObject* wtype = Js_Type(w);

	if (vtype == wtype)
	{
		return (vtype->tp_compare(v, w) < 0) ? Js_True : Js_False;
	}

	return Js_False;

	double vdouble = JsNum_GetDouble(v);
	double wdouble = JsNum_GetDouble(w);

	return (vdouble < wdouble) ? Js_True : Js_False;
}
JsObject* op_gt(JsObject* v, JsObject* w)
{
	JsTypeObject* vtype = Js_Type(v);
	JsTypeObject* wtype = Js_Type(w);

	if (vtype == wtype)
	{
		return (vtype->tp_compare(v, w) > 0) ? Js_True : Js_False;
	}

	double vdouble = JsNum_GetDouble(v);
	double wdouble = JsNum_GetDouble(w);

	return (vdouble > wdouble) ? Js_True : Js_False;
}
JsObject* op_le(JsObject* v, JsObject* w)
{
	JsTypeObject* vtype = Js_Type(v);
	JsTypeObject* wtype = Js_Type(w);

	if (vtype == wtype)
	{
		return (vtype->tp_compare(v, w) > 0) ? Js_False : Js_True;
	}

	double vdouble = JsNum_GetDouble(v);
	double wdouble = JsNum_GetDouble(w);

	return (vdouble > wdouble) ? Js_False : Js_True;
}
JsObject* op_ge(JsObject* v, JsObject* w)
{
	JsTypeObject* vtype = Js_Type(v);
	JsTypeObject* wtype = Js_Type(w);

	if (vtype == wtype)
	{
		return (vtype->tp_compare(v, w) < 0) ? Js_False : Js_True;
	}

	double vdouble = JsNum_GetDouble(v);
	double wdouble = JsNum_GetDouble(w);

	return (vdouble < wdouble) ? Js_False : Js_True;
}

int op_assign(JsObject* v, JsObject* w)
{
	return 0;
}