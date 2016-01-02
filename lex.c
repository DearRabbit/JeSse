#include "lex.h"
#include "utility.h"
#include <ctype.h>
#include <string.h>

#define TOKEN_MAX_LENGTH 100

// v : The global lexer( maybe not static?)
// v * Change in runtime environment causes a lexer
// v * stack manipulation.
int token;
char sToken[TOKEN_MAX_LENGTH];
static int currCh;
static int nextCh;
static int currPos;	// data[currPos] == currCh
static int dataEnd;

static const char* data;
// static Lexer l;

// lexer initializer
void lxInit(char* buf)
{
	assert(buf != NULL);
	dataEnd = strlen(buf);
	currPos = 0;
	data = buf;
	getNextCh();
	getNextCh();
	gettok();
}

void lxReset(int pos)	// pos may be specified by some function definition
{
	currPos = pos;
	getNextCh();
	getNextCh();
	gettok();
}

// TODO: 
void lxPush()
{

}

void lxPop()
{

}

// Do a comparation and get the next token.
void match(int expected_tk)
{
	if( token != expected_tk )
	{
		FATAL_ERROR("Got %s expected %s at %s\n", 
						getTokStr(token), getTokStr(expected_tk), getPosition());
	}
	gettok();
}

// Get the next token
void gettok()
{
	while( currCh && isblank(currCh) )
	{
		getNextCh();
	}
	// TODO:
}

static int getLineCnt()
{
	// TODO:
	return 0;
}

static void getNextCh()
{
	currCh = nextCh;
	if( nextCh < dataEnd )
		nextCh = data[currPos];
	else
		nextCh = 0;

	currPos ++; 
}

static char* getTokStr(int tk)
{
	switch (tk)
	{
        case LEX_EOF : return "EOF";
        case LEX_ID : return "ID";
        case LEX_INT : return "INT";
        case LEX_FLOAT : return "FLOAT";
        case LEX_STR : return "STRING";
        case LEX_EQUAL : return "==";
        case LEX_TYPEEQUAL : return "===";
        case LEX_NEQUAL : return "!=";
        case LEX_NTYPEEQUAL : return "!==";
        case LEX_LEQUAL : return "<=";
        case LEX_LSHIFT : return "<<";
        case LEX_LSHIFTEQUAL : return "<<=";
        case LEX_GEQUAL : return ">=";
        case LEX_RSHIFT : return ">>";
        case LEX_RSHIFTUNSIGNED : return ">>";
        case LEX_RSHIFTEQUAL : return ">>=";
        case LEX_PLUSEQUAL : return "+=";
        case LEX_MINUSEQUAL : return "-=";
        case LEX_PLUSPLUS : return "++";
        case LEX_MINUSMINUS : return "--";
        case LEX_ANDEQUAL : return "&=";
        case LEX_ANDAND : return "&&";
        case LEX_OREQUAL : return "|=";
        case LEX_OROR : return "||";
        case LEX_XOREQUAL : return "^=";
        // reserved words
        case LEX_R_IF : return "if";
        case LEX_R_ELSE : return "else";
        case LEX_R_DO : return "do";
        case LEX_R_WHILE : return "while";
        case LEX_R_FOR : return "for";
        case LEX_R_BREAK : return "break";
        case LEX_R_CONTINUE : return "continue";
        case LEX_R_FUNCTION : return "function";
        case LEX_R_RETURN : return "return";
        case LEX_R_VAR : return "var";
        case LEX_R_TRUE : return "true";
        case LEX_R_FALSE : return "false";
        case LEX_R_NULL : return "null";
        case LEX_R_UNDEFINED : return "undefined";
        case LEX_R_NEW : return "new";
	}

}
