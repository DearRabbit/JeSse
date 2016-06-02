#include "runtime.h"

uhash
_Js_HashPointer(void *p)
{
    uhash x;
    size_t y = (size_t)p;
    /* bottom 3 or 4 bits are likely to be 0; rotate y by 4 to avoid
       excessive hash collisions for dicts and sets */
    y = (y >> 4) | (y << (8 * sizeof(void*) - 4));
    x = (uhash)y;
    return x;
}

uhash
JsObject_Hash(JsObject *v)
{
    JsTypeObject *tp = v->ob_type;
    if (tp->tp_hash != NULL)
        return (*tp->tp_hash)(v);

    /* Use address as hash value */
    return _Js_HashPointer(v);
}

void
donut_dealloc(JsObject *obj)
{
    // do nothing.
}