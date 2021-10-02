
#ifndef GREEKLATINGSQ_ARRAYINT_H
#define GREEKLATINGSQ_ARRAYINT_H


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

int* atArrayInt(ArrayInt * obj, int i);


#endif //GREEKLATINGSQ_ARRAYINT_H
