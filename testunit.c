#include <stdio.h>

#include "intobject.h"
#include "runtime.h"

extern JsIntObject* small_ints[];

int main()
{
	int a = _JsInt_Init();
	printf("%ld\n", small_ints[0]->ob_ival);
	return 0;
}