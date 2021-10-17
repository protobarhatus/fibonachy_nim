#include "Array2dInt.h"
#include "assert.h"
#include "string.h"


static void assignFunctions(Array2dInt * obj)
{
    obj->at = &atArray2dInt;
    obj->cat = &catArray2dInt;
    obj->getWidth = &matrix2dIntGetWidth;
    obj->getHeight = &matrix2dIntGetHeight;
}

struct  Array2d_struct defaultArray2dInt(int w, int h, int def_val)
{
    struct Array2d_struct arr;

    assignFunctions(&arr);
    assert(w <= ARR_MAX_WIDTH);
    assert(h <= ARR_MAX_HEIGHT);
    assert(w >= 0);
    assert(h >= 0);

    arr.w = w;
    arr.h = h;
    memset((void *)&arr.arr, def_val, sizeof(int) * ARR_MAX_HEIGHT * ARR_MAX_WIDTH);
    return arr;
}

int * atArray2dInt(struct  Array2d_struct * obj, int x, int y);


const int * catArray2dInt(const Array2dInt * obj, int x, int y);


int findElementOnVertical(const Array2dInt * arr, int x, int el)
{
    for (int i = 0; i < arr->h; ++i)
    {
        if (*catArray2dInt(arr, i, x) == el)
            return i;
    }
    return -1;
}
Array2dInt makeIndexesWayArray(const Array2dInt * arr)
{
    Array2dInt res = defaultArray2dInt(arr->w, arr->h, 0);

    for (int i = 0; i < arr->w; ++i)
    {
        for (int j = 0; j < arr->h; ++j)
        {
            int ind = findElementOnVertical(arr, (i + 1) % arr->w, *catArray2dInt(arr, j, i));
            assert(ind >= 0);
            *atArray2dInt(&res, i, j) = ind;
        }
    }
    return res;
}

int matrix2dIntGetWidth(const Array2dInt * arr)
{
    return arr->w;
}
int matrix2dIntGetHeight(const Array2dInt * arr)
{
    return arr->h;
}