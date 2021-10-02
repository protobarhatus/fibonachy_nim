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

int* atArrayInt(ArrayInt * obj, int i)
{
    assert(i >= 0);
    assert(i < obj->n);

    return &obj->arr[i];
}
