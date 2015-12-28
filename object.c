#include "object.h"

long
_Js_HashPointer(void *p)
{
    long x;
    size_t y = (size_t)p;
    /* bottom 3 or 4 bits are likely to be 0; rotate y by 4 to avoid
       excessive hash collisions for dicts and sets */
    y = (y >> 4) | (y << (8 * sizeof(void*) - 4));
    x = (long)y;
    if (x == -1)
        x = -2;
    return x;
}