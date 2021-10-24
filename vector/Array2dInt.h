#ifndef GREEKLATINGSQ_ARRAY2DINT_H
#define GREEKLATINGSQ_ARRAY2DINT_H

#include "arrayint.h"

#define ARR_MAX_WIDTH ARR_INT_MAX_SIZE

#define ARR_MAX_HEIGHT ARR_INT_MAX_SIZE

//<"=" == copy>
//<no move>
struct Array2d_struct
{
    int w, h;
    int arr[ARR_MAX_WIDTH * ARR_MAX_HEIGHT];

    int * (*at)(struct Array2d_struct * obj, int x, int y);
    const int * (*cat)(const struct  Array2d_struct * obj, int x, int y);
    int (*getWidth)(const struct  Array2d_struct * obj);
    int (*getHeight)(const struct  Array2d_struct * obj);

};

typedef struct Array2d_struct Array2dInt;

struct  Array2d_struct defaultArray2dInt(int w, int h, int def_val);

static inline int * atArray2dInt(struct  Array2d_struct * obj, int x, int y)
{
    assert(x < obj->w);
    assert(y < obj->h);
    assert(x >= 0);
    assert(y >= 0);

    return &obj->arr[x * ARR_MAX_WIDTH + y];
}

static inline const int * catArray2dInt(const Array2dInt * obj, int x, int y)
{
    assert(x < obj->w);
    assert(y < obj->h);
    assert(x >= 0);
    assert(y >= 0);

    return &obj->arr[x * ARR_MAX_WIDTH + y];
}

int matrix2dIntGetWidth(const Array2dInt * arr);
int matrix2dIntGetHeight(const Array2dInt * arr);

Array2dInt makeIndexesWayArray(const Array2dInt * arr);
#endif //GREEKLATINGSQ_ARRAY2DINT_H
