#ifndef EXACT_COVER_VECTOR_H
#define EXACT_COVER_VECTOR_H

#include "vectordef.h"
#include "vectorfuncs.h"


#define MAKE_VECTOR(TN, UCN) \
MAKE_VEC(TN, UCN)            \
MAKE_VECTOR_FUNCTIONS(TN, UCN)


MAKE_VECTOR(int, Int);
MAKE_VECTOR(char, Char);
MAKE_VECTOR(double, Double);
MAKE_VECTOR(bool, Bool);


MAKE_VECTOR(VectorInt, VectorInt);

MAKE_VECTOR(VectorVectorInt, VectorVectorInt);

MAKE_VECTOR(VectorBool, VectorBool)



#endif //EXACT_COVER_VECTOR_H
