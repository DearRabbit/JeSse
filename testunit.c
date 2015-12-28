#include <stdio.h>

#include "intobject.h"

int main()
{
	_Js_Dealloc(&JsInt_Type);
	return 0;
}