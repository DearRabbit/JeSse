#include <assert.h>
#include "runtime.h"
#include "jsast.h"
#include "jesse.h"

static JsFuncObject* global_instance;
static JsFuncObject* current_instance;
static int continue_flag=0;
static int break_flag=0;
static int return_flag=0;

static JsObject* evaluate(nodeType* stmt);

static JsObject* op_binary(nodeType* op1, nodeType* op2, binaryfunc op_func)
{
	JsObject* obj1 = evaluate(op1);
	JsObject* obj2 = evaluate(op2);
	JsObject* result = op_func(obj1, obj2);
	Js_DECREF(obj1); Js_DECREF(obj2);
	return result;
}

static JsObject* evaluate(nodeType* stmt)
{
	if(!stmt) return Js_Null;	// empty stmt

							// v: for assignments cannot do this
	if(stmt->type == IDLET) // get variable from string
	{
		JsObject* name = JsString_FromString(stmt->strval);
		JsObject* var = JsFunc_GetVariable(current_instance, name);
		Js_DECREF(name);
		Js_INCREF(var);
		return var;
	}
	if(stmt->type == STRLET)
		return JsString_FromString(stmt->strval);	// Can be NULL
	if(stmt->type == NUMLET)
		return JsNum_FromDouble(stmt->numval);
	if(stmt->type == BOOLLET)
		return JsBool_FromInt(stmt->numval);
	
	int i;
	switch(stmt->opr.oper)
	{
		case OP_STMTS:
		{
			JsObject* ret;
			for(i=0; i<stmt->opr.nops; i++)
			{
				if( continue_flag || break_flag || return_flag ) break;
				ret = evaluate(stmt->opr.op[i]);
			}
			return ret;
		}
		case OP_RETURN:
		{
			JsObject* ret;
			if(stmt->opr.nops)
				ret = evaluate(stmt->opr.op[0]);
			else
				ret = Js_Null;
			return_flag = 1;
			return ret;
		}
		case OP_IF:
		{
			JsObject* cond = evaluate(stmt->opr.op[0]);
			int boolean = JsBool_GetBool(cond);
			Js_DECREF(cond);
			if(boolean)
				return evaluate(stmt->opr.op[1]);
			return Js_Null;
		}
		case OP_IFELSE:
		{
			JsObject* cond = evaluate(stmt->opr.op[0]);
			int boolean = JsBool_GetBool(cond);
			Js_DECREF(cond);
			if(boolean)
				return evaluate(stmt->opr.op[1]);
			else
				return evaluate(stmt->opr.op[2]);
		}
		case OP_BREAK:
		{

		}
			break;
		case OP_FOR:
		{

		}
			break;
		case OP_WHILE:
		{
			JsObject* ret = Js_Null;
			JsObject* cond = evaluate(stmt->opr.op[0]);
			int boolean = JsBool_GetBool(cond);
			Js_DECREF(cond);
			while(boolean)
			{
				ret = evaluate(stmt->opr.op[1]);
				if(return_flag)
					return ret;
				cond = evaluate(stmt->opr.op[0]);
				boolean = JsBool_GetBool(cond);
			}
			return Js_Null;
		}
		case OP_FUNCTION:
		{
			nodeType* nodeName = stmt->opr.op[0];
			JsObject* name = JsString_FromString(nodeName->strval);
			JsObject* def = JsDef_New(current_instance, (JsAst*)(stmt));
			JsFunc_SetVariable(current_instance, name, def);
			return Js_Null;
		}
		case OP_DECLARATION:
		{
			for(i=0; i<stmt->opr.nops; i++)
				evaluate(stmt->opr.op[i]);
			return Js_Null;
		}
		case OP_DECLARATOR:
		{
			if(stmt->opr.nops == 1)
			{
				nodeType* nodeName = stmt->opr.op[0];
				JsObject* name = JsString_FromString(nodeName->strval);
				JsFunc_DefVariable(current_instance, name, Js_Undefined);
				Js_DECREF(name);
				return Js_Null;
			}
			else
			{
				// lhs_expression can be a literal...or a number...how strange
				nodeType* nodeName = stmt->opr.op[0];
				JsObject* name = JsString_FromString(nodeName->strval);
				JsObject* val = evaluate(stmt->opr.op[1]);
				JsFunc_DefVariable(current_instance, name, val);
				Js_DECREF(name);
				Js_DECREF(val);
				return Js_Null;
			}
		}
		case OP_ARRAY:
		{

		}
		case OP_ASSIGN:
		{
			nodeType* nodeName = stmt->opr.op[0];
			JsObject* name = JsString_FromString(nodeName->strval);
			JsObject* val = evaluate(stmt->opr.op[1]);
			JsFunc_SetVariable(current_instance, name, val);
			Js_DECREF(name);
			Js_DECREF(val);
			return val;
		}
		case OP_TERNARY:
		{

		}
			break;
		case OP_OROR:
		{
			return op_binary(stmt->opr.op[0], stmt->opr.op[1], op_logic_or);
		}
		case OP_ANDAND:
		{
			return op_binary(stmt->opr.op[0], stmt->opr.op[1], op_logic_and);
		}
		case OP_BITOR:
		{
			return op_binary(stmt->opr.op[0], stmt->opr.op[1], op_or);
		}
		case OP_BITXOR:
		{
			return op_binary(stmt->opr.op[0], stmt->opr.op[1], op_xor);
		}
		case OP_BITAND:
		{
			return op_binary(stmt->opr.op[0], stmt->opr.op[1], op_and);
		}
		case OP_EQ:
		{
			return op_binary(stmt->opr.op[0], stmt->opr.op[1], op_eq);
		}
		case OP_LT:
		{
			return op_binary(stmt->opr.op[0], stmt->opr.op[1], op_lt);
		}
		case OP_GT:
		{
			return op_binary(stmt->opr.op[0], stmt->opr.op[1], op_gt);
		}
		case OP_LE:
		{
			return op_binary(stmt->opr.op[0], stmt->opr.op[1], op_le);
		}
		case OP_GE:
		{
			return op_binary(stmt->opr.op[0], stmt->opr.op[1], op_ge);
		}
		case OP_NEQ:
		{
			return op_binary(stmt->opr.op[0], stmt->opr.op[1], op_neq);
		}
		case OP_INSTANCEOF:
		{

		}
			break;
		case OP_IN:
		{

		}
			break;
		case OP_LSHIFT:
		{

		}
			break;
		case OP_RSHIFT:
		{

		}
			break;
		case OP_RSHIFTU:
		{

		}
			break;
		case OP_ADD:
		{
			return op_binary(stmt->opr.op[0], stmt->opr.op[1], op_add);
		}
		case OP_MINUS:
		{
			return op_binary(stmt->opr.op[0], stmt->opr.op[1], op_sub);
		}
		case OP_MUL:
		{
			return op_binary(stmt->opr.op[0], stmt->opr.op[1], op_mul);
		}
		case OP_DIV:
		{
			return op_binary(stmt->opr.op[0], stmt->opr.op[1], op_div);
		}
		case OP_LPPLUS:
		{
			nodeType* nameNode = stmt->opr.op[0];
			if( nameNode->type != IDLET)
				Js_FatalError("Error: Self-increase operation cannot perform on a constant\n");
			JsObject* name = JsString_FromString(nameNode->strval);
			JsObject* one = JsNum_FromDouble(1.0);
			JsObject* var = evaluate(stmt->opr.op[0]);
			JsObject* ret = op_add(var, one);
			Js_DECREF(one);
			JsFunc_SetVariable(current_instance, name, ret);
			Js_DECREF(name);
			return ret;
		}
		case OP_LMMINUS:
		{
			nodeType* nameNode = stmt->opr.op[0];
			if( nameNode->type != IDLET)
				Js_FatalError("Error: Self-decrease operation cannot perform on a constant\n");
			JsObject* name = JsString_FromString(nameNode->strval);
			JsObject* one = JsNum_FromDouble(1.0);
			JsObject* var = evaluate(stmt->opr.op[0]);
			JsObject* ret = op_sub(var, one);
			Js_DECREF(one);
			JsFunc_SetVariable(current_instance, name, ret);
			Js_DECREF(name);
			return ret;
		}
		case OP_PREMINUX:
		{
			JsObject* ret = evaluate(stmt->opr.op[0]);
			// TODO: reverse sign
			return ret;
		}
		case OP_TILDE:
		{

		}
			break;
		case OP_NOT:
		{

		}
			break;
		case OP_RPPLUS:
		{
			JsObject* one = JsNum_FromDouble(1.0);
			JsObject* var = evaluate(stmt->opr.op[0]);
			JsObject* ret = op_add(var, one);
			Js_DECREF(one);
			return ret;
		}
			break;
		case OP_RMMINUS:
		{

		}
			break;
		case OP_MEMBER:
		{

		}
			break;
		case OP_PARAMETERS:
		{
			// For function declaration, evaluated in OP_FUNCTION
			assert(0);
		}
			break;
		case OP_CALL:
		{
			if( strcmp( stmt->opr.op[0]->strval, "print" ) == 0)
			{
				JsObject* arg = evaluate(stmt->opr.op[1]->opr.op[0]);
				Js_PRINTLNOBJ(arg);
				Js_DECREF(arg);
				return Js_Undefined;
			}
			// JsObject* func_name = evaluate(stmt->opr.op[0]);
			// JsObject* func_def = JsFunc_GetVariable(current_instance, func_name);
			JsObject* func_def = evaluate(stmt->opr.op[0]);
			JsObject* func_instance = JsDef_NewInstance(func_def);
			nodeType* func_root = (nodeType*)JsDef_GetAst(func_def);
			nodeType* func_para = func_root->opr.op[1];
			nodeType* call_arg = stmt->opr.op[1];
			// Js_DECREF(func_name);

			JsObject* para_name;
			JsObject* arg;
			for(i=0; i<func_para->opr.nops; i++)
			{
				para_name = JsString_FromString(func_root->opr.op[1]->opr.op[i]->strval);
				arg = evaluate(call_arg->opr.op[i]);
				if(!arg) arg = Js_Undefined;
				// !!!set current_instance
				JsFunc_DefVariable((JsFuncObject*)func_instance, para_name, arg);
				Js_DECREF(para_name);
				Js_DECREF(arg);
			}
			JsFuncObject* last_instance = current_instance;
			current_instance = (JsFuncObject*)func_instance;
			JsObject* ret = evaluate(func_root->opr.op[2]);
			current_instance = last_instance;
			return_flag = 0;
			return ret;
		}
		case OP_OBJECT:
		{

		}
			break;
		case OP_PROPERTY:
		{

		}
			break;
		case OP_ARGUMENTS:
		{
			// For function calling. Evaluated in OP_CALL
			assert(0);
		}
			break;
		case OP_CONTINUE:
		{

		}
			break;
		case OP_SWITCH:
		{

		}
			break;
		case OP_FUNCTION_EXPR:
		{
			return JsDef_New(current_instance, (JsAst*)stmt);
		}
	}	//: switch(stmt->opr.oper)
	Js_FatalError("Unrecognized operation\n");
}

//   v: All things are done here
void init()
{
	_JsNum_Init();
	_JsBool_Init();
	_JsBaseVar_Init();
	_JsDict_Init();

	// unusual way of initialization.
	JsFuncObject* alloc = Js_Malloc(sizeof(JsFuncObject));
	global_instance = alloc;
	current_instance = alloc;

	JsObject_INIT(alloc, &JsFunc_Type);
	// no need to create a global definition.
	alloc->func_def = NULL;
	alloc->var_table = (JsDictObject*)JsDict_New();
}

void deinit()
{
	_JsDict_Deinit();
	_JsBaseVar_Deinit();
	_JsBool_Deinit();
	_JsNum_Deinit();
}

void execute(nodeType* root)
{
	assert(root != NULL);
	assert(root->type == OPRLET && root->opr.oper == OP_STMTS);
	init();
	evaluate(root);
	Js_PRINTLNOBJ(global_instance->var_table);
	deinit();
	Js_Exit();
}

