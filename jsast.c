#include <stdlib.h>
#include <stdarg.h>
#include "jsast.h"
#include "runtime.h"

nodeType* num(double a_num)
{
    nodeType* p;

    if( (p=(nodeType*)malloc(sizeof(nodeType))) == NULL )
        Js_FatalError("out of memory\n");

    p->type = NUMLET;
    p->numval = a_num;

    return p;
}
nodeType* id(char* a_id)
{
    nodeType* p;

    if( (p = (nodeType*)malloc(sizeof(nodeType)) ) == NULL )
        Js_FatalError("out of memory\n");

    p->type = IDLET;
    p->strval = a_id;

    return p;
}
nodeType* str(char* a_str)
{
    nodeType* p;

    if( (p = (nodeType*)malloc(sizeof(nodeType)) ) == NULL )
        Js_FatalError("out of memory\n");

    p->type = STRLET;
    p->strval = a_str;

    return p;
}
nodeType* bull(int a_bool)
{
    nodeType* p;

    if( (p=(nodeType*)malloc(sizeof(nodeType))) == NULL )
        Js_FatalError("out of memory\n");

    p->type = BOOLLET;
    p->numval = a_bool;

    return p;
}
nodeType* opr(int oper, int nops, ...)
{
    va_list ap;
    nodeType* p;
    int i;

    if( (p=(nodeType*)malloc(sizeof(nodeType))) == NULL)
        Js_FatalError("out of memory\n");
    if( (p->opr.op=(nodeType**)malloc(sizeof(nodeType*) * nops)) == NULL)
        Js_FatalError("out of memory\n");

    p->type = OPRLET;
    p->opr.oper = (operEnum)oper;
    p->opr.nops = nops;

    va_start(ap, nops);
    for (i=0; i<nops; i++)
        p->opr.op[i] = va_arg(ap, nodeType*);
    va_end(ap);
    return p;
}

nodeType* add(nodeType* p, nodeType* son)
{
    p->opr.nops++;
    p->opr.op = (nodeType**)realloc(p->opr.op, (p->opr.nops) * sizeof(nodeType*) );
    if(!p->opr.op)
        dbgprint("out of memory\n");
    p->opr.op[p->opr.nops-1] = son;
    return p;
}

void freeNode(nodeType* p)
{
    int i;
    if(!p) return;

    if(p->type == OPRLET)
    {
        for(i=0; i<p->opr.nops; i++)
        {
            freeNode(p->opr.op[i]);
        }
        free(p->opr.op);
    }
    if(p->type == STRLET || p->type == IDLET)
        free(p->strval);

    free(p);
}

#ifdef __cplusplus
#include <list>
using std::list;
void AstPrint(nodeType* root)
{
    if(!root)
        return;
    list<nodeType*> l;
    l.push_back(root);
    int level_count=1;
    int next_level=0;
    while(!l.empty())
    {
        nodeType* n = l.front(); l.pop_front();
        if(n)
        {
            if(n->type == OPRLET)
                for(int i=0; i<n->opr.nops; i++)
                {
                    next_level++;
                    l.push_back(n->opr.op[i]);
                }
            if(n->type == OPRLET)
                printf("oper:%d(%d) |", n->opr.oper, n->opr.nops);
            else if(n->type == STRLET || n->type == IDLET)
                printf("\"%s\" |", n->strval);
            else
                printf("%g |", n->numval);
        }
        else
            printf("NULL| ");
        level_count--;
        if(!level_count)
        {
            level_count = next_level;
            next_level = 0;
            printf("\n");
        }
    }
}
#endif