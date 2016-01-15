#include "jesse.h"

int main(int argc, char** argv)
{
	extern FILE *yyin, *yyout;
	yyin = fopen(argv[1], "r");
	// yyout = fopen("parse_spit.txt", "w");
	if(!yyin)
	{
		fprintf(stderr, "File not found: %s\n", argv[1]);
		return -1;
	}
	nodeType* rootAst;
	yyparse(&rootAst);
#ifdef JS_DEBUG
	dbgprint("parse complete\n");
#endif
	// AstPrint(rootAst);
	execute(rootAst);

	freeNode(rootAst);
	fclose(yyin);
	// fclose(yyout);

	return 0;
}
