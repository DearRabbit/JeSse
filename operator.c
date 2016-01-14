#include "runtime.h"

#include <math.h>

JsObject*
op_add(JsObject* v, JsObject* w)
{
	// bool & int
	if (JsNum_CheckSubClass(v) && JsNum_CheckSubClass(w))
	{
		double op1 = JsNum_GetDouble(v);
		double op2 = JsNum_GetDouble(w);
		double result = op1 + op2;

		return JsNum_FromDouble(result);
	}
	else
	{
		JsObject* strv = JsString_GetString(v);
		JsObject* strw = JsString_GetString(w);
		JsObject* strres = _JsString_StringJoin(strv, strw);

		if (!JsString_CheckType(v))
			Js_DECREF(strv);
		if (!JsString_CheckType(w))
			Js_DECREF(strw);

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

static JsObject* 
_internal_op_eq(JsObject* v, JsObject* w)
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

JsObject* op_eq(JsObject* v, JsObject* w)
{
	JsObject* ret = _internal_op_eq(v, w);
	Js_INCREF(ret);
	return ret;
}
JsObject* op_neq(JsObject* v, JsObject* w)
{
	JsObject* oppo = _internal_op_eq(v, w);
	JsObject* ret = (oppo == Js_False) ? Js_True : Js_False;

	Js_INCREF(ret);
	return ret;
}
JsObject* op_lt(JsObject* v, JsObject* w)
{
	JsTypeObject* vtype = Js_Type(v);
	JsTypeObject* wtype = Js_Type(w);
	JsObject* ret;
	
	if (vtype == wtype)
	{
		ret = (vtype->tp_compare(v, w) < 0) ? Js_True : Js_False;
		Js_INCREF(ret);
		return ret;
	}

	double vdouble = JsNum_GetDouble(v);
	double wdouble = JsNum_GetDouble(w);

	ret = (vdouble < wdouble) ? Js_True : Js_False;
	Js_INCREF(ret);
	return ret;
}
JsObject* op_gt(JsObject* v, JsObject* w)
{
	JsTypeObject* vtype = Js_Type(v);
	JsTypeObject* wtype = Js_Type(w);
	JsObject* ret;
	
	if (vtype == wtype)
	{
		ret = (vtype->tp_compare(v, w) > 0) ? Js_True : Js_False;
		Js_INCREF(ret);
		return ret;
	}

	double vdouble = JsNum_GetDouble(v);
	double wdouble = JsNum_GetDouble(w);

	ret = (vdouble > wdouble) ? Js_True : Js_False;
	Js_INCREF(ret);
	return ret;
}
JsObject* op_le(JsObject* v, JsObject* w)
{
	JsTypeObject* vtype = Js_Type(v);
	JsTypeObject* wtype = Js_Type(w);
	JsObject* ret;

	if (vtype == wtype)
	{
		ret = (vtype->tp_compare(v, w) > 0) ? Js_False : Js_True;
		Js_INCREF(ret);
		return ret;
	}

	double vdouble = JsNum_GetDouble(v);
	double wdouble = JsNum_GetDouble(w);

	ret = (vdouble > wdouble) ? Js_False : Js_True;
	Js_INCREF(ret);
	return ret;
}
JsObject* op_ge(JsObject* v, JsObject* w)
{
	JsTypeObject* vtype = Js_Type(v);
	JsTypeObject* wtype = Js_Type(w);
	JsObject* ret;

	if (vtype == wtype)
	{
		ret = (vtype->tp_compare(v, w) < 0) ? Js_False : Js_True;
		Js_INCREF(ret);
		return ret;
	}

	double vdouble = JsNum_GetDouble(v);
	double wdouble = JsNum_GetDouble(w);

	ret = (vdouble < wdouble) ? Js_False : Js_True;
	Js_INCREF(ret);
	return ret;
}

JsObject* op_logic_and(JsObject* v, JsObject* w)
{
	int valv = JsBool_GetBool(v);
	int valw = JsBool_GetBool(w);
	JsObject* ret;

	ret = (valv && valw) ? Js_True : Js_False;
	Js_INCREF(ret);
	return ret;
}
JsObject* op_logic_or(JsObject* v, JsObject* w)
{
	int valv = JsBool_GetBool(v);
	int valw = JsBool_GetBool(w);
	JsObject* ret;

	ret = (valv || valw) ? Js_True : Js_False;
	Js_INCREF(ret);
	return ret;
}