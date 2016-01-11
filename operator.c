#include "runtime.h"

JsObject*
op_add(JsObject* v, JsObject* w)
{
	if (JsNum_CheckType(v) && JsNum_CheckType(w))
	{
		double op1 = JsNum_GetDouble(v);
		double op2 = JsNum_GetDouble(w);
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