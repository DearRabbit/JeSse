%{
#include "jsast.h"
#include "yacc.tab.h"

%}
LineContinuation \\(\r\n|\r|\n)
SingleEscapeCharacter [\'\"\\bfnrtv]
NonEscapeCharacter [^\'\"\\bfnrtv0-9xu]
CharacterEscapeSequence (SingleEscapeCharacter)|(NonEscapeCharacter)
EscapeSequence (CharacterEscapeSequence)
DoubleStringCharacter ([^\"\\\n\r]+)|(\\{EscapeSequence})|{LineContinuation} 
SingleStringCharacter ([^\'\\\n\r]+)|(\\{EscapeSequence})|{LineContinuation} 
StringLiteral (\"{DoubleStringCharacter}*\")|(\'{SingleStringCharacter}*\') 

%option nounput

%%
break		{return BREAK;}
export		{return EXPORT;}
super		{return SUPER;}
case		{return CASE;}
extends		{return EXTENDS;}
switch		{return SWITCH;}
class		{return CLASS;}
finally		{return FINALLY;}
this		{return THIS;}
catch		{return CATCH;}
for			{return FOR;}
throw		{return THROW;}
const 		{return CONST;}
function	{return FUNCTION;}
try			{return TRY;}
continue	{return CONTINUE;}
if 			{return IF;}
typeof		{return TYPEOF;}
debugger	{return DEBUGGER;}
import		{return IMPORT;}
var 		{dbgprint("At var\n"); return VAR;}
default		{return DEFAULT;}
in 			{return IN;}
void		{return VOID;}
delete		{return DELETE;}
instanceof	{return INSTANCEOF;}
while		{return WHILE;}
do			{return DO;}
new			{return NEW;}
with		{return WITH;}
else		{return ELSE;}
return		{return RETURN;}
yield		{return YIELD;}
true		{return TRUE;}
false		{return FALSE;}
null		{return NULLPTR;}
undefined	{return UNDEFINED;}

[A-Za-z_][A-Za-z0-9_]*	{ 
							dbgprint("At identifier:%s\n", yytext);
							STRDUP_NEW(yylval.strval, yytext);
							return IDENTIFIER; 
						}

{StringLiteral}		{
						yylval.strval = (char*)malloc(strlen(yytext)-1);
						strncpy(yylval.strval, yytext+1, strlen(yytext)-2);
						return STRINGLITERAL;
					}

[-&~|^*/%<>(){},.!?:]	{ dbgprint("%c\n", yytext[0]); return yytext[0]; }

[0-9]+	|
[0-9]+"."[0-9]*	|
"."[0-9]+			|
[0-9]+[eE][-+]?[0-9]+				|
[0-9]+"."[0-9]*[eE][-+]?[0-9]+	|
"."[0-9]+[eE][-+]?[0-9]+	{ 
								dbgprint("At number:%s\n", yytext);
								yylval.numval = atof(yytext);  
								return NUMBER; 
							}

"="			{dbgprint("At \"=\"\n");return yytext[0];}
";"			{dbgprint("At ;\n"); return yytext[0];}
"+"			{dbgprint("At +\n"); return yytext[0];}
"["			return yytext[0];
"]"			return yytext[0];

"==="		{return TYPEEQUAL;}
"=="		{return EQUAL;}
"!=="		{return TYPENEQUAL;}
"!="		{return NEQUAL;}
"<<="		{return LSHIFTEQUAL;}
"<<"		{return LSHIFT;}
"<="		{return LEQUAL;}
">>>="		{return RSHIFTUNSIGNEDEQUAL;}
">>>"		{return RSHIFTUNSIGNED;}
">>="		{return RSHIFTEQUAL;}
">>"		{return RSHIFT;}
">="		{return GEQUAL;}
"+="		{return PLUSEQUAL;}
"++"		{return PLUSPLUS;}
"-="		{return MINUSEQUAL;}
"--"		{return MINUSMINUS;}
"*="		{return MULTIPLYEQUAL;}
"/="		{return DIVIDEEQUAL;}
"%="		{return REMAINEQUAL;}
"&&"		{return ANDAND;}
"&="		{return ANDEQUAL;}
"||"		{return OROR;}
"|="		{return OREQUAL;}
"^="		{return XOR;}
"//".*(\r\n|\r|\n) {dbgprint("At //\n");}
"/*"(.|\r|\n)*"*/" {dbgprint("At /*\n");}
"\n"		{dbgprint("At \\n\n"); }
[ \t\r]+	{dbgprint("At empty\n");}
<<EOF>>		{dbgprint("At EOF\n"); return ENDOFFILE;}
.			{dbgprint("Undefined\n");return ERRORTOKEN;}

%%

int yywrap()
{
	return 1;
}