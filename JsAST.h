#pragma once
#include <string.h>
#include "utils.h"

#define STRDUP_NEW(dest, src) \
{\
	dest = (char*) malloc(strlen(src)+1),\
	strcpy(dest, src);\
}

typedef enum {
	OP_STMTS,
	OP_RETURN,
	OP_IF,
	OP_IFELSE,
	OP_BREAK,
	OP_FOR,
	OP_WHILE,
	OP_FUNCTION,
	OP_DECLARATION,
	OP_DECLARATOR,
	OP_ARRAY,
	OP_ASSIGN,
	OP_TERNARY,
	OP_OROR,
	OP_ANDAND,
	OP_BITOR,
	OP_BITXOR,
	OP_BITAND,
	OP_EQ,
	OP_LT,
	OP_GT,
	OP_LE,
	OP_GE,
	OP_INSTANCEOF,
	OP_IN,
	OP_LSHIFT,
	OP_RSHIFT,
	OP_RSHIFTU,
	OP_ADD,
	OP_MINUS,
	OP_MUL,
	OP_DIV,
	OP_LPPLUS,
	OP_LMMINUS,
	OP_PREMINUX,
	OP_TILDE,
	OP_NOT,
	OP_RPPLUS,
	OP_RMMINUS,
	OP_MEMBER,
	OP_PARAMETERS,
	OP_CALL,
	OP_OBJECT,
	OP_PROPERTY,
	OP_ARGUMENTS,
	OP_CONTINUE,
	OP_SWITCH,
	OP_FUNCTION_EXPR
} operEnum;	// operations

typedef enum  {
	IDLET,
	STRLET,	// --v:
	NUMLET,	// leaf nodes
	OPRLET
} nodeEnum;

typedef struct {
	operEnum oper;
	int nops;
	struct _nodeType** op;
} oprNode;

typedef struct _nodeType {
	nodeEnum type;
	union {
		char* strval;
		double numval;
		oprNode opr;
	};
} nodeType;

// new operation node
nodeType* opr(int oper, int nops, ...);
// new number node
nodeType* num(double a_num);
// new string node
nodeType* str(char* a_str);
// new id node
nodeType* id(char* a_id);
// add son to node, returns the node
nodeType* add(nodeType* node, nodeType* son);
// free abstract syntax tree
void freeNode(nodeType* root);
// printTree
void AstPrint(nodeType* root);