#ifndef EXACT_COVER_VECTORFUNCS_H
#define EXACT_COVER_VECTORFUNCS_H
#include "vectordef.h"

void sortVector(Vector * vec, int (*comparator)(const void * a, const void * b));


#define MAKE_VECTOR_FUNCTIONS(TN, UCN)                                                                                              \
static inline void sortVector##UCN(Vector##UCN * obj, int (*comparator)(const void *, const void *)) {                                \
    sortVector(&obj->vec, comparator);                                                                                       \
 }                                                                                                                              \
                                                                                                                            \

#endif //EXACT_COVER_VECTORFUNCS_H
