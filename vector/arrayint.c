#include <stdio.h>
#include "arrayint.h"
#include <assert.h>
#include <string.h>

static void assignFunctions(ArrayInt * obj)
{
    obj->at = &atArrayInt;
    obj->cat = &catArrayInt;
    obj->getSize = &arrayIntGetSize;
    obj->canPushBack = &arrayIntCanPushBack;
    obj->pushBack = &arrayIntPushBack;
}

ArrayInt defaultArrayInt(int n, int def_val)
{
    ArrayInt res;

    assignFunctions(&res);
    assert(n <= ARR_INT_MAX_SIZE);
    assert(n >= 0);

    res.n = n;
    memset(&res.arr, def_val, sizeof(int) * n);

    return res;
}

void printfArrayInt(ArrayInt * obj)
{
    for (int i  =0 ; i < obj->n; ++i)
        printf("%d ", *atArrayInt(obj, i));
    printf("\n");
}

int arrayIntGetSize(const ArrayInt * obj)
{
    return obj->n;
}

bool arrayIntCanPushBack(const ArrayInt * obj)
{
    return obj->n < ARR_INT_MAX_SIZE;
}

void arrayIntPushBack(ArrayInt * obj, int element)
{
    assert(arrayIntCanPushBack(obj));
    *obj->at(obj, obj->n) = element;
    obj->n++;
}