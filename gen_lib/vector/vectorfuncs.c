#include "vectorfuncs.h"
#include "vectordef.h"

void sortVector(Vector * vec, int (*comparator)(const void * a, const void * b))
{
    qsort(vec->vec, vec->size, vec->type->element_size, comparator);
}