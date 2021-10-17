
#ifndef GREEKLATINGSQ_ARRAYINT_H
#define GREEKLATINGSQ_ARRAYINT_H


#include <assert.h>
#include <stdbool.h>

#define ARR_INT_MAX_SIZE 11

//<"=" == copy>
//<no move>
struct ArrayInt_struct
{
    int arr[ARR_INT_MAX_SIZE];
    int n;

    int* (*at)(struct ArrayInt_struct* obj, int i);
    const int* (*cat)(const struct ArrayInt_struct* obj, int i);
    int (*getSize)(const struct ArrayInt_struct* obj);
    bool (*canPushBack)(const struct ArrayInt_struct * obj);
    void (*pushBack)(struct ArrayInt_struct * obj, int element);
};

typedef struct ArrayInt_struct ArrayInt;
typedef struct ArrayInt_struct ArrayPointer;

ArrayInt defaultArrayInt(int n, int def_val);

static inline int* atArrayInt(ArrayInt * obj, int i)
{
    assert(i >= 0);
    assert(i < obj->n);
    return &obj->arr[i];
}

static inline const int* catArrayInt(const ArrayInt * obj, int i)
{
    assert(i >= 0);
    assert(i < obj->n);
    return &obj->arr[i];
}

int arrayIntGetSize(const ArrayInt * obj);

void printfArrayInt(ArrayInt * obj);

bool arrayIntCanPushBack(const ArrayInt * obj);

void arrayIntPushBack(ArrayInt * obj, int element);


#endif //GREEKLATINGSQ_ARRAYINT_H
