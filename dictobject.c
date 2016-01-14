#include "runtime.h"

#include <stdlib.h>
#include <string.h>
#include <assert.h>

static JsObject *dummy;

// forward declarations
static JsDictEntry*
lookdict_string(JsDictObject *mp, JsObject *key, uhash hash);

#define EMPTY_TO_MINSIZE(mp) do {									\
	memset((mp)->ma_smalltable, 0, sizeof((mp)->ma_smalltable));	\
	(mp)->ma_used = (mp)->ma_fill = 0;								\
	(mp)->ma_table = (mp)->ma_smalltable;							\
	(mp)->ma_mask = JsDict_MINSIZE - 1;								\
	} while(0)

// use free list
#define JsDict_MAXFREELIST 64
static JsDictObject *free_list[JsDict_MAXFREELIST];
static int numfree;

JsObject *
JsDict_New(void)
{
	register JsDictObject *mp;
	if (dummy == NULL) { /* Auto-initialize dummy */
		dummy = JsString_FromString("<dummy key>");
	}
	if (numfree) {
		mp = free_list[--numfree];
		_Js_NewReference((JsObject *)mp);
	} else {
		mp = Js_Malloc(sizeof(JsDictObject));
		JsObject_INIT(mp, &JsDict_Type);
	}
	EMPTY_TO_MINSIZE(mp);
	mp->ma_lookup = lookdict_string;
	return (JsObject *)mp;
}

#define PERTURB_SHIFT 5

static JsDictEntry*
lookdict_string(JsDictObject *mp, JsObject *key, uhash hash)
{
	register size_t i;
	register size_t perturb;
	register JsDictEntry *freeslot;
	register size_t mask = (size_t)mp->ma_mask;
	JsDictEntry *ep0 = mp->ma_table;
	register JsDictEntry *ep;

    i = hash & mask;
    ep = &ep0[i];
    if (ep->me_key == NULL || ep->me_key == key)
        return ep;
    if (ep->me_key == dummy)
        freeslot = ep;
    else {
        if (ep->me_hash == hash && _JsString_Eq(ep->me_key, key))
            return ep;
        freeslot = NULL;
    }

    /* In the loop, me_key == dummy is by far (factor of 100s) the
       least likely outcome, so test for that last. */
    for (perturb = hash; ; perturb >>= PERTURB_SHIFT) {
        i = (i << 2) + i + perturb + 1;
        ep = &ep0[i & mask];
        if (ep->me_key == NULL)
            return freeslot == NULL ? ep : freeslot;
        if (ep->me_key == key
            || (ep->me_hash == hash
            && ep->me_key != dummy
            && _JsString_Eq(ep->me_key, key)))
            return ep;
        if (ep->me_key == dummy && freeslot == NULL)
            freeslot = ep;
    }
    assert(0);          /* NOT REACHED */
    return 0;
}

/*
Internal routine to insert a new item into the table.
Used both by the internal resize routine and by the public insert routine.
Eats a reference to key and one to value.
Returns -1 if an error occurred, or 0 on success.
*/
static int
insertdict(JsDictObject *mp, JsObject *key, uhash hash, JsObject *value)
{
	register JsDictEntry *ep;
	JsObject* old_value;

	assert(mp->ma_lookup != NULL);
	ep = mp->ma_lookup(mp, key, hash);
	if (ep == NULL) {
        // never reach
        Js_DECREF(key);
        Js_DECREF(value);
		return -1;
	}
	if (ep->me_value != NULL) {
		old_value = ep->me_value;
		ep->me_value = value;
		Js_DECREF(old_value);
		Js_DECREF(key);
	}
	else {
		if (ep->me_key == NULL)
			// unused state
			mp->ma_fill++;
		else {
			// dummy state
			assert(ep->me_key == dummy);
			Js_DECREF(dummy);
		}
		ep->me_key = key;
		ep->me_hash = hash;
		ep->me_value = value;
		mp->ma_used++;
	}
	return 0;
}

/*
Internal routine used by dictresize() to insert an item which is
known to be absent from the dict.  This routine also assumes that
the dict contains no deleted entries.
*/
static void
insertdict_clean(register JsDictObject *mp, JsObject *key, uhash hash,
                 JsObject *value)
{
    register size_t i;
    register size_t perturb;
    register size_t mask = (size_t)mp->ma_mask;
    JsDictEntry *ep0 = mp->ma_table;
    register JsDictEntry *ep;

    i = hash & mask;
    ep = &ep0[i];
    for (perturb = hash; ep->me_key != NULL; perturb >>= PERTURB_SHIFT) {
        i = (i << 2) + i + perturb + 1;
        ep = &ep0[i & mask];
    }
	assert(ep->me_value == NULL);
	mp->ma_fill++;
	ep->me_key = key;
	ep->me_hash = hash;
	ep->me_value = value;
	mp->ma_used++;
}

static int
dictresize(JsDictObject *mp, ssize_t minused)
{
    ssize_t newsize;
    JsDictEntry *oldtable, *newtable, *ep;
    ssize_t i;
    int is_oldtable_malloced;
    JsDictEntry small_copy[JsDict_MINSIZE];

    assert(minused >= 0);

    /* Find the smallest table size > minused. */
    for (newsize = JsDict_MINSIZE;
         newsize <= minused && newsize > 0;
         newsize <<= 1)
        ;
    if (newsize <= 0) {
        dbgprint("dict resize error");
        return -1;
    }

    /* Get space for a new table. */
    oldtable = mp->ma_table;
    assert(oldtable != NULL);
    is_oldtable_malloced = oldtable != mp->ma_smalltable;

    if (newsize == JsDict_MINSIZE) {
        /* A large table is shrinking, or we can't get any smaller. */
        newtable = mp->ma_smalltable;
        if (newtable == oldtable) {
            if (mp->ma_fill == mp->ma_used) {
                /* No dummies, so no point doing anything. */
                return 0;
            }
            /* We're not going to resize it, but rebuild the
               table anyway to purge old dummy entries.
               Subtle:  This is *necessary* if fill==size,
               as lookdict needs at least one virgin slot to
               terminate failing searches.  If fill < size, it's
               merely desirable, as dummies slow searches. */
            assert(mp->ma_fill > mp->ma_used);
            memcpy(small_copy, oldtable, sizeof(small_copy));
            oldtable = small_copy;
        }
    }
    else {
        newtable = Js_Malloc(sizeof(JsDictEntry) * newsize);
    }

    /* Make the dict empty, using the new table. */
    assert(newtable != oldtable);
    mp->ma_table = newtable;
    mp->ma_mask = newsize - 1;
    memset(newtable, 0, sizeof(JsDictEntry) * newsize);
    mp->ma_used = 0;
    i = mp->ma_fill;
    mp->ma_fill = 0;

    /* Copy the data over; this is refcount-neutral for active entries;
       dummy entries aren't copied over, of course */
    for (ep = oldtable; i > 0; ep++) {
        if (ep->me_value != NULL) {             /* active entry */
            --i;
            insertdict_clean(mp, ep->me_key, (long)ep->me_hash,
                             ep->me_value);
        }
        else if (ep->me_key != NULL) {          /* dummy entry */
            --i;
            assert(ep->me_key == dummy);
            Js_DECREF(ep->me_key);
        }
        /* else key == value == NULL:  nothing to do */
    }

    if (is_oldtable_malloced)
        Js_Free(oldtable);
    return 0;
}

JsObject *
JsDict_GetItem(JsObject *op, JsObject *key)
{
    uhash hash;
    JsDictObject *mp = (JsDictObject *)op;
    JsDictEntry *ep;
    if (!JsString_CheckType(key) ||
        (hash = ((JsStringObject *) key)->ob_shash) == 0)
    {
        hash = JsObject_Hash(key);
        //// nullstring 
        // if (hash == 0) {
        //     return NULL;
        // }
    }

	ep = (mp->ma_lookup)(mp, key, hash);
	if (ep == NULL) {
        // never reach
        assert(0);
		return NULL;
	}
	return ep->me_value;
}

static int
dict_set_item_by_hash(JsObject *op, JsObject *key,
                               uhash hash, JsObject *value)
{
    register JsDictObject *mp;
    register ssize_t n_used;

    mp = (JsDictObject *)op;
    assert(mp->ma_fill <= mp->ma_mask);  /* at least one empty slot */
    n_used = mp->ma_used;
    Js_INCREF(value);
    Js_INCREF(key);
    if (insertdict(mp, key, hash, value) != 0)
        return -1;

    /* If we added a key, we can safely resize.  Otherwise just return!
     * If fill >= 2/3 size, adjust size.  Normally, this doubles or
     * quaduples the size, but it's also possible for the dict to shrink
     * (if ma_fill is much larger than ma_used, meaning a lot of dict
     * keys have been * deleted).
     *
     * Quadrupling the size improves average dictionary sparseness
     * (reducing collisions) at the cost of some memory and iteration
     * speed (which loops over every possible entry).  It also halves
     * the number of expensive resize operations in a growing dictionary.
     *
     * Very large dictionaries (over 50K items) use doubling instead.
     * This may help applications with severe memory constraints.
     */
    if (!(mp->ma_used > n_used && mp->ma_fill*3 >= (mp->ma_mask+1)*2))
        return 0;
    return dictresize(mp, (mp->ma_used > 50000 ? 2 : 4) * mp->ma_used);
}

/* CAUTION: JsDict_SetItem() must guarantee that it won't resize the
 * dictionary if it's merely replacing the value for an existing key.
 */
int
JsDict_SetItem(JsObject *op, JsObject *key, JsObject *value)
{
    uhash hash;

    if (!JsDict_CheckType(op)) {
        return -1;
    }
    assert(key);
    assert(value);
    if (JsString_CheckType(key)) {
        hash = ((JsStringObject *)key)->ob_shash;
        if (hash == 0)
            hash = JsObject_Hash(key);
    }
    else {
        // never reach
        hash = JsObject_Hash(key);
    }
    return dict_set_item_by_hash(op, key, hash, value);
}

int
JsDict_DelItem(JsObject *op, JsObject *key)
{
    register JsDictObject *mp;
    register uhash hash;
    register JsDictEntry *ep;
    JsObject *old_value, *old_key;

    if (!JsDict_CheckType(op)) {
        return -1;
    }
    assert(key);
    if (!JsString_CheckType(key) ||
        (hash = ((JsStringObject *) key)->ob_shash) == 0) {
        hash = JsObject_Hash(key);
    }
    mp = (JsDictObject *)op;
    ep = (mp->ma_lookup)(mp, key, hash);
    if (ep == NULL)
        return -1;
    if (ep->me_value == NULL) {
        return -1;
    }
    old_key = ep->me_key;
    Js_INCREF(dummy);
    ep->me_key = dummy;
    old_value = ep->me_value;
    ep->me_value = NULL;
    mp->ma_used--;
    Js_DECREF(old_value);
    Js_DECREF(old_key);
    return 0;
}

void
JsDict_Clear(JsObject *op)
{
    JsDictObject *mp;
    JsDictEntry *ep, *table;
    int table_is_malloced;
    ssize_t fill;
    JsDictEntry small_copy[JsDict_MINSIZE];

    if (!JsDict_CheckType(op))
        return;
    mp = (JsDictObject *)op;

    table = mp->ma_table;
    assert(table != NULL);
    table_is_malloced = table != mp->ma_smalltable;

    /* This is delicate.  During the process of clearing the dict,
     * decrefs can cause the dict to mutate.  To avoid fatal confusion
     * (voice of experience), we have to make the dict empty before
     * clearing the slots, and never refer to anything via mp->xxx while
     * clearing.
     */
    fill = mp->ma_fill;
    if (table_is_malloced)
        EMPTY_TO_MINSIZE(mp);

    else if (fill > 0) {
        /* It's a small table with something that needs to be cleared.
         * Afraid the only safe way is to copy the dict entries into
         * another small table first.
         */
        memcpy(small_copy, table, sizeof(small_copy));
        table = small_copy;
        EMPTY_TO_MINSIZE(mp);
    }
    /* else it's a small table that's already empty */

    /* Now we can finally clear things.  If C had refcounts, we could
     * assert that the refcount on table is 1 now, i.e. that this function
     * has unique access to it, so decref side-effects can't alter it.
     */
    for (ep = table; fill > 0; ++ep) {
        if (ep->me_key) {
            --fill;
            Js_DECREF(ep->me_key);
            Js_XDECREF(ep->me_value);
        }
    }

    if (table_is_malloced)
        Js_Free(table);
}

static JsObject*
dict_new(JsTypeObject *type, JsObject *args)
{
	return JsDict_New();
}

static void
dict_dealloc(JsDictObject* mp)
{
	register JsDictEntry *ep;
	ssize_t fill = mp->ma_fill;
	for (ep = mp->ma_table; fill > 0; ep++) {
		if (ep->me_key) {
		--fill;
			Js_DECREF(ep->me_key);
			Js_XDECREF(ep->me_value);
		}
	}
	if (mp->ma_table != mp->ma_smalltable)
	{
		Js_Free(mp->ma_table);
	}
	if (numfree < JsDict_MAXFREELIST && Js_Type(mp) == &JsDict_Type)
		free_list[numfree++] = mp;
	else
		Js_Free(mp);
}

JsTypeObject JsDict_Type = {
	JsObject_HEAD_INIT(&JsType_Type)
	"object",			// in Javascript, it's object instead of dict
	sizeof(JsDictObject),
	0,

	JS_TPFLAGS_DEFAULT, // not a basetype

	(newfunc)dict_new,					/* tp_new, also generate by api.*/
	(destructor)dict_dealloc,			/* tp_dealloc */
	//(printfunc)dict_print,				/* tp_print */
	//(tostringfunc)dict_tostring,		/* tp_tostr */
	NULL,
	NULL,

	NULL,								/* tp_compare */
	(hashfunc)_Js_HashPointer,			/* tp_hash, use the common version */
};

int _JsDict_Init(void)
{
    return 0;
}

void _JsDict_Deinit(void)
{
    ssize_t i;
    for (i = numfree - 1; i >= 0; --i)
    {
        // if there's a outer table, it has been freed in dealloc.
        if (free_list[i] != NULL)
            free(free_list[i]);
    }
    if (dummy != NULL)
    {
        _Js_Dealloc(dummy);
    }
}
