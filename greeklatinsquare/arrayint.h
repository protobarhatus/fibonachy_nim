
#ifndef GREEKLATINGSQ_ARRAYINT_H
#define GREEKLATINGSQ_ARRAYINT_H


#include <assert.h>

#define ARR_INT_MAX_SIZE 11

//<"=" == copy>
//<no move>
struct ArrayInt_struct
{
    int arr[ARR_INT_MAX_SIZE];
    int n;
};

typedef struct ArrayInt_struct ArrayInt;

ArrayInt defaultArrayInt(int n, int def_val);

static inline int* atArrayInt(ArrayInt * obj, int i)
{
    assert(i >= 0);
    assert(i < obj->n);
    return &obj->arr[i];
}

void printfArrayInt(ArrayInt * obj);


#endif //GREEKLATINGSQ_ARRAYINT_H
