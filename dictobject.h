#pragma once

#include "object.h"

#define JsDict_MINSIZE 8

typedef struct {
    /* Cached hash code of me_key. */
    uhash me_hash;
    JsObject *me_key;
    JsObject *me_value;
} JsDictEntry;

typedef struct _dictobject JsDictObject;
struct _dictobject {
    JsObject_HEAD
    ssize_t ma_fill;  /* # Active + # Dummy */
    ssize_t ma_used;  /* # Active */

    /* The table contains ma_mask + 1 slots, and that's a power of 2.
     * We store the mask instead of the size because the mask is more
     * frequently needed.
     */
    ssize_t ma_mask;

    /* ma_table points to ma_smalltable for small tables, else to
     * additional malloc'ed memory.  ma_table is never NULL!  This rule
     * saves repeated runtime null-tests in the workhorse getitem and
     * setitem calls. 
     */
    JsDictEntry *ma_table;

    /* currently use string as key */
    JsDictEntry *(*ma_lookup)(JsDictObject *mp, JsObject *key, uhash hash);
    JsDictEntry ma_smalltable[JsDict_MINSIZE];
};

extern JsTypeObject JsDict_Type;

#define JsDict_CheckType(obj) ((obj)->ob_type == &JsDict_Type)

// C - API
JsObject * JsDict_New(void);
JsObject * JsDict_GetItem(JsObject *op, JsObject *key);
// return -1 if error
int JsDict_SetItem(JsObject *op, JsObject *key, JsObject *value);
int JsDict_DelItem(JsObject *op, JsObject *key);
void JsDict_Clear(JsObject *op);


