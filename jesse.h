#pragma once
#include "jsast.h"
#include "runtime.h"
// yyparse
extern void yyparse(nodeType **root);
// execute
void execute(nodeType* root);
