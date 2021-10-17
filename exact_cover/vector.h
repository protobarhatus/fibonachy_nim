#ifndef EXACT_COVER_VECTOR_H
#define EXACT_COVER_VECTOR_H

#include "vectordef.h"
#include "vectorfuncs.h"

#define MAKE_VECTOR(TN, UCN, FUNC_MAC) \
MAKE_VEC(TN, UCN, FUNC_MAC)            \
MAKE_VECTOR_FUNCTIONS(TN, UCN)


MAKE_VECTOR(int, Int, SIMPLE_TYPE);
MAKE_VECTOR(char, Char, SIMPLE_TYPE);
MAKE_VECTOR(double, Double, SIMPLE_TYPE);
MAKE_VECTOR(bool, Bool, SIMPLE_TYPE);


MAKE_VECTOR(VectorInt, VectorInt, STRUCT_TYPE(VectorInt));

MAKE_VECTOR(VectorVectorInt, VectorVectorInt, STRUCT_TYPE(VectorVectorInt));

MAKE_VECTOR(VectorBool, VectorBool, STRUCT_TYPE(VectorBool))

//call in any .c file one time per every MAKE_VEC under SIMPLE_TYPE defined
#define GEN_DUMMY_FUNCS_FOR_SIMPLE_TYPE(TN, UCN) TN copy##UCN(const TN * b) {return *b;} void destruct##UCN(TN * a) {} TN move##UCN(TN * b) {return *b;}
//call in any .c file one time per every MAKE_VEC under POINTER_TYPE defined
#define GEN_DUMMY_FUNCS_FOR_POINTER_TYPE(TN, UCN) TN copy##UCN(const TN * b) {return *b;} TN move##UCN(TN * b) {return *b;}

#endif //EXACT_COVER_VECTOR_H
