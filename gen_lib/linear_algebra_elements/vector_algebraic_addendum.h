#ifndef FIBONACHY_NIM_VECTOR_ALGEBRAIC_ADDENDUM_H
#define FIBONACHY_NIM_VECTOR_ALGEBRAIC_ADDENDUM_H
#include "../vector/vector.h"
#include "../types_and_functions_declarations/arithmetic_type_declaration.h"
#include "../types_and_functions_declarations/type_declaration.h"
#include "../types_and_functions_declarations/functions_extensions.h"
//to make vector algebra functions for type TN, need to declare its as type, as arithmetic_type and
//then generate functions here
//unfortunately, vector can not be declared as arithmetic type because it will have to define many unsutable for it functions
//and arithmetic of vector of vector is described in matrix and it differs.
void addToVector(Vector * dest, const Vector * b, const ArithmeticTypePresenter * artype);
void subToVector(Vector * dest, const Vector * b, const ArithmeticTypePresenter * artype);

Vector addVector(const Vector * a, const Vector * b, const ArithmeticTypePresenter * artype);
Vector subVector(const Vector * a, const Vector * b, const ArithmeticTypePresenter * artype);

void multVectorOnNum(Vector * dest, const void * el,  const ArithmeticTypePresenter * artype);
Vector vectorMultedOnNum(const Vector * vec, const void * el,  const ArithmeticTypePresenter * artype);

void * scalar(const Vector * a, const Vector * b, const ArithmeticTypePresenter * artype);

void * lengthVector(const Vector * a, const ArithmeticTypePresenter * artype);
void * distanceVector(const Vector * a, const Vector * b, const ArithmeticTypePresenter * artype);



#define MAKE_VECTOR_ALGEBRAIC_ADDENDUM(TN, UCN) \
static inline void addToVector##UCN(Vector##UCN * dest, const Vector##UCN * b)\
{\
    addToVector(&dest->vec, &b->vec, ARITHM_TYPE_##UCN());\
}                                               \
static inline void subToVector##UCN(Vector##UCN * dest, const Vector##UCN * b)\
{                                               \
    subToVector(&dest->vec, &b->vec, ARITHM_TYPE_##UCN());\
}                                               \
static inline Vector##UCN addVector##UCN(const Vector##UCN * a, const Vector##UCN * b) { \
    Vector vr = addVector(&a->vec, &b->vec, ARITHM_TYPE_##UCN());                         \
    return createVector##UCN##FromVector(&vr);\
}                                               \
static inline Vector##UCN subVector##UCN(const Vector##UCN * a, const Vector##UCN * b) { \
    Vector vr = subVector(&a->vec, &b->vec, ARITHM_TYPE_##UCN());             \
    return createVector##UCN##FromVector(&vr);\
}                                               \
static inline void multVector##UCN##OnNum(Vector##UCN * a, const TN * num) {  \
    multVectorOnNum(&a->vec, (const void*)num, ARITHM_TYPE_##UCN());                                                \
}                                               \
static inline Vector##UCN vector##UCN##MultedOnNum(const Vector##UCN * a, const TN * num) {                   \
    Vector vr = vectorMultedOnNum(&a->vec, (const void *)num, ARITHM_TYPE_##UCN()); \
    return createVector##UCN##FromVector(&vr);  \
}                                               \
static inline TN scalarVector##UCN(const Vector##UCN * a, const Vector##UCN * b) {      \
    TN * res = (TN*)scalar(&a->vec, &b->vec, ARITHM_TYPE_##UCN());             \
    return *res;\
}                                               \
static inline TN lengthVector##UCN(const Vector##UCN * a) {                        \
    TN * res = (TN*)lengthVector(&a->vec, ARITHM_TYPE_##UCN());               \
    return *res;\
}                                               \
static inline TN distanceVector##UCN(const Vector##UCN * a, const Vector##UCN * b) {    \
    TN * res = (TN*)distanceVector(&a->vec, &b->vec, ARITHM_TYPE_##UCN());    \
    return *res;\
}                                               \
GENERATE_RIGHT_VALUE_BINARY_EXTENSIONS(Vector##UCN, Vector##UCN, add, Vector##UCN)         \
GENERATE_RIGHT_VALUE_BINARY_EXTENSIONS(Vector##UCN, Vector##UCN, sub, Vector##UCN)                                                \
GENERATE_RIGHT_VALUE_BINARY_EXTENSIONS(Vector##UCN, Vector##UCN, scalar, TN)  \
GENERATE_RIGHT_VALUE_BINARY_EXTENSIONS(Vector##UCN, Vector##UCN, distance, TN)\
GENERATE_RIGHT_VALUE_UNARY_EXTENSIONS(Vector##UCN, Vector##UCN, length, TN)                                                \
                                                \


MAKE_VECTOR_ALGEBRAIC_ADDENDUM(int, Int);
MAKE_VECTOR_ALGEBRAIC_ADDENDUM(double, Double);

#endif //FIBONACHY_NIM_VECTOR_ALGEBRAIC_ADDENDUM_H
