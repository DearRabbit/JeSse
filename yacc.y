%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "jsast.h"

extern int yylex();
extern char* yytext;
extern FILE* yyin;

int yyerror( nodeType** p, char* s)
{
	fprintf(stderr, "error: %s\n", s);
	return 0;
}
%}

%parse-param { nodeType** YYAST }

%union{
	char* strval;
	double numval;
	nodeType* node;
}

%token <strval> IDENTIFIER
%token <strval> STRINGLITERAL
%token <numval> NUMBER

%token BREAK
%token EXPORT
%token SUPER
%token CASE
%token EXTENDS
%token SWITCH
%token CLASS
%token FINALLY
%token THIS
%token CATCH
%token FOR
%token THROW
%token CONST
%token FUNCTION
%token TRY
%token CONTINUE
%token IF
%token TYPEOF
%token DEBUGGER
%token IMPORT
%token VAR
%token DEFAULT
%token IN
%token VOID
%token DELETE
%token INSTANCEOF
%token WHILE
%token DO
%token NEW
%token WITH
%token ELSE
%token RETURN
%token YIELD
%token TRUE
%token FALSE
%token NULLPTR
%token UNDEFINED

%token TYPEEQUAL
%token EQUAL
%token TYPENEQUAL
%token NEQUAL
%token LSHIFTEQUAL
%token LSHIFT
%token LEQUAL
%token RSHIFTUNSIGNEDEQUAL
%token RSHIFTUNSIGNED
%token RSHIFTEQUAL
%token RSHIFT
%token GEQUAL
%token PLUSEQUAL
%token PLUSPLUS
%token MINUSEQUAL
%token MINUSMINUS
%token MULTIPLYEQUAL
%token DIVIDEEQUAL
%token REMAINEQUAL
%token ANDAND
%token ANDEQUAL
%token OROR
%token OREQUAL
%token XOR
%token ENDOFFILE
%token ERRORTOKEN
%token ENDOFLINE

%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE
%left GEQUAL LEQUAL EQUAL NEQUAL '>' '<'
%left '+' '-'
%left '*' '/'
%nonassoc UMINUS

%type <node> program
	stmt_list stmt stmt_variable stmt_block 
	stmt_expression stmt_declaration stmt_if
	stmt_for stmt_while stmt_continue stmt_break
	stmt_return stmt_switch variable_declarationList
	variable_declarator variable_initializer assignment_expression 
	function_declaration function_body parameter_list array_literal
	object_literal conditional_expression or_expression and_expression bit_or_expression bit_xor_expression bit_and_expression equal_expression relational_expression additive_expression shift_expression multiply_expression prefix_expression unary_expression postfix_expression lhs_expression call_expression member_expression primary_expression function_expression object_list property_assignment arguments element_list property_name
%%
program:
		stmt_list
		{dbgprint("At program\n"); *YYAST = $1; return 0;}
	;

stmt_list:
		stmt 
		{
			dbgprint("At stmt_list\n");
			$$ = opr(OP_STMTS, 1, $1);
		}
	|	stmt_list stmt 
		{
			$$ = add($1, $2);
		}
	//|	{dbgprint("At Empty\n"); $$ = NULL;}/* empty stmt */
	;

stmt:
		stmt_block
	|	stmt_variable
	|	stmt_expression
	|	stmt_declaration
	|	stmt_if
	|	stmt_for
	|	stmt_while
	|	stmt_continue
	|	stmt_break
	|	stmt_return
	|	stmt_switch
	;

stmt_return:
		RETURN assignment_expression ';'
		{
			dbgprint("At stmt_return\n");
			$$ = opr(OP_RETURN, 1, $2);
		}
	|	RETURN ';'
		{
			dbgprint("At stmt_return\n");
			$$ = opr(OP_RETURN, 0);
		}
	;
stmt_if:
		IF '(' assignment_expression ')' stmt %prec LOWER_THAN_ELSE
		{
			$$ = opr(OP_IF, 2, $3, $5);
		}
	|	IF '(' assignment_expression ')' stmt ELSE stmt
		{
			$$ = opr(OP_IFELSE, 3, $3, $5, $7);
		}
	;
stmt_break:
		BREAK ';'
		{
			$$ = opr(OP_BREAK, 0);
		}
	;
/* do not support comma for simplicity */
stmt_expression:
		assignment_expression ';'
		{
			dbgprint("At stmt_expression\n");
		}
	;
/* only the c-style for-loop for simplicity */
stmt_for:
		FOR '(' assignment_expression ';' assignment_expression ';' assignment_expression ')' stmt
		{
			$$ = opr(OP_FOR, 4, $3, $5, $7, $9);
		}
	;
stmt_while:
		WHILE '(' assignment_expression ')' stmt
		{
			$$ = opr(OP_WHILE, 2, $3, $5);
		}
	;
stmt_continue:
		CONTINUE ';'
		{
			$$ = opr(OP_CONTINUE, 0);
		}
	;
/* maybe later */
stmt_switch:
		SWITCH ';'
		{
			$$ = opr(OP_SWITCH, 0);
		}
	;
stmt_declaration:
		function_declaration
	;

function_declaration:
		FUNCTION IDENTIFIER '(' ')' '{' function_body '}'
		{
			$$ = opr(OP_FUNCTION, 3, id($2), opr(OP_PARAMETERS, 0), $6);
		}
	|	FUNCTION IDENTIFIER '(' parameter_list ')' '{' function_body '}'
		{
			dbgprint("At function_declaration\n");
			$$ = opr(OP_FUNCTION, 3, id($2), $4, $7);
		}
	;

stmt_block:
		'{' stmt_list '}'
		{
			dbgprint("At stmt_block\n");
			$$ = $2;
		}
	;

stmt_variable:
		VAR variable_declarationList ';'
		{
			dbgprint("At stmt_variable\n");
			$$ = $2;
		}
	;

variable_declarationList:
		variable_declarator
		{
			$$ = opr(OP_DECLARATION, 1, $1);
		}
	|	variable_declarationList ',' variable_declarator
		{
			$$ = add($1, $3);
		}
	;

variable_declarator:
		IDENTIFIER 
		{
			$$ = opr(OP_DECLARATOR, 1, id($1) );
		}
	|	IDENTIFIER variable_initializer
		{
			/*          OP_DECLARATOR	(similar to OP_ASSIGN)
			 *			/			\
			 *			v 			v
			 *			ID 			OP_ADD (a special case of expression)
			 *						/	\
			 *						a 	b
			 */
			$$ = opr(OP_DECLARATOR, 2, id($1), $2 );
		}
	;

variable_initializer:
		'=' assignment_expression
		{
			$$ = $2;
		}
	|	'=' array_literal
		{	
			/*			OP_DECLARATOR
			 *			/			\
			 *			v 			v 
			 *			ID 			OP_ARRAY
			 *						/  / \  \
			 *						a 'b' 1  true
			 */
			$$ = $2;
		}
	|	'=' object_literal
		{
			$$ = $2;
		}
	;

assignment_expression:
		conditional_expression
		{
			dbgprint("At assignment_expression\n");
		}
	|	lhs_expression '=' assignment_expression
		{
			dbgprint("At assignment_expression\n");
			$$ = opr(OP_ASSIGN, 2, $1, $3);
		}
	;

conditional_expression:
		or_expression
	|	or_expression '?' assignment_expression ':' assignment_expression
		{
			$$ = opr(OP_TERNARY, 3, $1, $3, $5);
		}
	;

or_expression:
		and_expression
	|	or_expression OROR and_expression
		{
			$$ = opr(OP_OROR, 2, $1, $3);
		}
	;

and_expression:
		bit_or_expression
	|	and_expression ANDAND bit_or_expression
		{
			$$ = opr(OP_ANDAND, 2, $1, $3);
		}
	;

bit_or_expression:
		bit_xor_expression
	|	bit_or_expression '|' bit_xor_expression
		{
			$$ = opr(OP_BITOR, 2, $1, $3);
		}
	;

bit_xor_expression:
		bit_and_expression
	|	bit_xor_expression '^' bit_and_expression
		{
			$$ = opr(OP_BITXOR, 2, $1, $3);
		}
	;

bit_and_expression:
		equal_expression
	|	bit_and_expression '&' equal_expression
		{
			$$ = opr(OP_BITAND, 2, $1, $3);
		}
	;

equal_expression:
		relational_expression
	|	equal_expression EQUAL relational_expression
		{
			$$ = opr(OP_EQ, 2, $1, $3);
		}
	;

relational_expression:
		shift_expression
	|	relational_expression '<' shift_expression
		{
			$$ = opr(OP_LT, 2, $1, $3);
		}
	|	relational_expression '>' shift_expression
		{
			$$ = opr(OP_GT, 2, $1, $3);
		}
	|	relational_expression LEQUAL shift_expression
		{
			$$ = opr(OP_LE, 2, $1, $3);
		}
	|	relational_expression GEQUAL shift_expression
		{
			$$ = opr(OP_GE, 2, $1, $3);
		}
	|	relational_expression NEQUAL shift_expression
		{
			$$ = opr(OP_NEQ, 2, $1, $3);
		}
	|	relational_expression INSTANCEOF shift_expression
		{
			$$ = opr(OP_INSTANCEOF, 2, $1, $3);
		}
	|	relational_expression IN shift_expression
		{
			$$ = opr(OP_IN, 2, $1, $3);
		}
	;

shift_expression:
		additive_expression
	|	shift_expression LSHIFT additive_expression
		{
			$$ = opr(OP_LSHIFT, 2, $1, $3);
		}
	|	shift_expression RSHIFT additive_expression
		{
			$$ = opr(OP_RSHIFT, 2, $1, $3);
		}
	|	shift_expression RSHIFTUNSIGNED additive_expression
		{
			$$ = opr(OP_RSHIFTU, 2, $1, $3);
		}
	;

additive_expression:
		multiply_expression
	|	additive_expression '+' multiply_expression
		{
			$$ = opr(OP_ADD, 2, $1, $3);
		}
	|	additive_expression '-' multiply_expression
		{
			$$ = opr(OP_MINUS, 2, $1, $3);
		}
	;

multiply_expression:
		unary_expression
	|	multiply_expression '*' unary_expression
		{
			$$ = opr(OP_MUL, 2, $1, $3);
		}
	|	multiply_expression '/' unary_expression
		{
			$$ = opr(OP_DIV, 2, $1, $3);
		}
	;

unary_expression:
		postfix_expression
	|	prefix_expression
	;

prefix_expression:
		PLUSPLUS unary_expression
		{
			$$ = opr(OP_LPPLUS, 1, $2);
		}
	|	MINUSMINUS unary_expression
		{
			$$ = opr(OP_LMMINUS, 1, $2);
		}
	|	'+' unary_expression
		{	// neglect
			$$ = $2;
		}
	|	'-' unary_expression
		{
			$$ = opr(OP_PREMINUX, 1, $2);
		}
	|	'~' unary_expression
		{	// TODO:
			$$ = opr(OP_TILDE, 1, $2);
		}
	|	'!' unary_expression
		{
			$$ = opr(OP_NOT, 1, $2);
		}
	;

postfix_expression:
		lhs_expression
	|	lhs_expression PLUSPLUS
		{
			$$ = opr(OP_RPPLUS, 1, $1);
		}
	|	lhs_expression MINUSMINUS
		{
			$$ = opr(OP_RMMINUS, 1, $1);
		}
	;

lhs_expression:
		member_expression
	|	call_expression
	;

member_expression:
		primary_expression
	|	function_expression
	|	member_expression '[' assignment_expression ']'
		{
			$$ = opr(OP_MEMBER, 2, $1, $3);
		}
	|	member_expression '.' IDENTIFIER
		{
			$$ = opr(OP_MEMBER, 2, $1, id($3));
		}
	;

function_expression:
		FUNCTION '(' ')' '{' function_body '}'
		{
			$$ = opr(OP_FUNCTION_EXPR, 3, NULL, opr(OP_PARAMETERS, 0), $5);
		}
	|	FUNCTION '(' parameter_list ')' '{' function_body '}'
		{
			$$ = opr(OP_FUNCTION_EXPR, 3, NULL, $3, $6);
		}
	;

call_expression:
		member_expression arguments
		{
			$$ = opr(OP_CALL, 2, $1, $2);
		}
	;

parameter_list:
		IDENTIFIER
		{
			dbgprint("At parameter_list\n");
			$$ = opr(OP_PARAMETERS, 1, id($1));
		}
	|	parameter_list ',' IDENTIFIER
		{
			$$ = add($1, id($3));
		}
	;

primary_expression:
		IDENTIFIER
		{
			$$ = id($1);
		}
	|	STRINGLITERAL
		{
			$$ = str($1);
		}
	|	NUMBER
		{
			$$ = num($1);
		}
	|	TRUE
		{
			$$ = bull(1);
		}
	|	FALSE
		{
			$$ = bull(0);
		}
	|	'(' assignment_expression ')'
		{
			$$ = $2;
		}
	;

object_literal:
		'{' '}'
		{
			$$ = opr(OP_OBJECT, 0);
		}
	|	'{' object_list '}'
		{
			$$ = $2;
		}
	|	'{' object_list ',' '}'
		{
			$$ = $2;
		}
	;

object_list:
		property_assignment		
		{
			$$ = opr(OP_OBJECT, 1, $1);
		}
	|	object_list ',' property_assignment
		{
			$$ = add($1, $3);
		}
	;

property_assignment:
		property_name ':' assignment_expression
		{
			$$ = opr(OP_PROPERTY, 2, $1, $3);
		}
	|	IDENTIFIER '(' ')' '{' function_body '}'
		{
			$$ = opr(OP_PROPERTY, 2, id($1), opr(OP_FUNCTION, 3, NULL, opr(OP_PARAMETERS, 0), $5));
		}
	|	IDENTIFIER '(' parameter_list ')' '{' function_body '}'
		{
			$$ = opr(OP_PROPERTY, 2, id($1), opr(OP_FUNCTION, 3, NULL, $3, $6));
		}
	;

function_body:
		stmt_list
		{	
			dbgprint("At function_body\n");
		}
	;

arguments:
		'(' ')'
		{
			$$ = opr(OP_ARGUMENTS, 0);
		}
	|	'(' element_list ')'
		{
			$$ = $2;
			$$->opr.oper = OP_ARGUMENTS;
		}
	;

property_name:
		IDENTIFIER
		{
			$$ = id($1);
		}
	|	STRINGLITERAL
		{
			$$ = str($1);
		}
	|	NUMBER
		{
			$$ = num($1);
		}
	;

array_literal:
		'[' ']'
		{
			$$ = opr(OP_ARRAY, 0);
		}
	|	'[' ',' ']'
		{
			$$ = opr(OP_ARRAY, 0);
		}
	|	'[' element_list ']'
		{
			$$ = $2;
		}
	|	'[' element_list ',' ']'
		{
			$$ = $2;
		}
	;

element_list:
		assignment_expression
		{
			$$ = opr(OP_ARRAY, 1, $1);
		}
	|	element_list ',' assignment_expression
		{
			$$ = add($1, $3);
		}
	;

%%