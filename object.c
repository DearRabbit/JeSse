#include "runtime.h"

u64
_Js_HashPointer(void *p)
{
	u64 x;
	size_t y = (size_t)p;
	/* bottom 3 or 4 bits are likely to be 0; rotate y by 4 to avoid
       excessive hash collisions for dicts and sets */
	y = (y >> 4) | (y << (8 * sizeof(void*) - 4));
	x = (u64)y;
	return x;
}