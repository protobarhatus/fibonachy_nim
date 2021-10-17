#ifndef EXACT_COVER_ALGORITHMS_H
#define EXACT_COVER_ALGORITHMS_H

#include "vector.h"

void printfVectorInt(const char * premessage, VectorInt * vec);

typedef int (*Comparator)(const void *, const void *);

int intLessComparator(const void * a, const void * b);

#define MAKE_PAIR(TN, UCN) \
struct Pair##UCN##_struct                           \
{                                                                   \
    TN first, second;                                               \
};                         \
typedef struct Pair##UCN##_struct Pair##UCN;        \
static inline Pair##UCN defaultPair##UCN(TN x, TN y) {              \
     Pair##UCN result;     \
     result.first = x;         \
     result.second = y;         \
     return result;        \
}



MAKE_PAIR(int, Int);
MAKE_VECTOR(PairInt, PairInt, SIMPLE_TYPE);
#endif //EXACT_COVER_ALGORITHMS_H
