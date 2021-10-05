#include <stdio.h>
#include "arrayint.h"
#include <assert.h>
#include <string.h>



ArrayInt defaultArrayInt(int n, int def_val)
{
    ArrayInt res;

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