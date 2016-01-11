#include "runtime.h"

#include <stdlib.h>

// Error On Exit, never return NULL
void*
Js_Malloc(size_t len)
{
	void *space = malloc(len);
	if (space == NULL)
	{
		Js_FatalError("Not enough memory");
	}
	return space;
}

void 
Js_Free(void* ptr)
{
	if (ptr != NULL)
		free(ptr);
}
