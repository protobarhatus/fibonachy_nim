#include "vectorfuncs.h"
#include "vectordef.h"

void sortVector(vector * vec, int (*comparator)(const void * a, const void * b))
{
    qsort(vec->vec, vec->size, vec->element_size, comparator);
}