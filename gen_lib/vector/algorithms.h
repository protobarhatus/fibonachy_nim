#ifndef EXACT_COVER_ALGORITHMS_H
#define EXACT_COVER_ALGORITHMS_H

#include "vector.h"

void printfVectorInt(const char * premessage, VectorInt * vec);
void printfVectorVectorInt(const char * premessage, VectorVectorInt * vec);
void scanfVectorInt(int size, VectorInt * vec);
void scanfVectorVectorInt(int lin, int col, VectorVectorInt * vec);
static inline int max(int a, int b)
{
    return a > b ? a : b;
}
static inline int min(int a, int b)
{
    return a < b ? a : b;
}
typedef int (*Comparator)(const void *, const void *);

int intLessComparator(const void * a, const void * b);

long long int gcd(long long int a, long long int b);



static inline bool areStringsEquall(const char * a, const char * b)
{
    return strcmp(a, b) == 0;
}


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
}                          \
static inline Pair##UCN copyPair##UCN(const Pair##UCN * cop) {    \
    Pair##UCN res;         \
    res.first = copy##UCN(&cop->first);             \
    res.second = copy##UCN(&cop->second);           \
    return res;             \
}                          \
static inline Pair##UCN movePair##UCN(Pair##UCN * cop) {          \
    Pair##UCN res;         \
    res.first = move##UCN(&cop->first);             \
    res.second = move##UCN(&cop->second);           \
    return res;     \
}                          \
static inline void destructPair##UCN(Pair##UCN * obj) {           \
    destruct##UCN(&obj->first);                     \
    destruct##UCN(&obj->second);\
}                           \
                           \
\
DECLARE_STRUCT_INLINE_TYPE(Pair##UCN, Pair##UCN);


MAKE_PAIR(int, Int);
MAKE_VECTOR(PairInt, PairInt);
#endif //EXACT_COVER_ALGORITHMS_H
