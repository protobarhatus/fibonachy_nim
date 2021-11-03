#ifndef EXACT_COVER_Vector_H
#define EXACT_COVER_Vector_H

#include <stddef.h>
#include "arrayint.h"
#include <stdlib.h>
#include <string.h>
#include "../types_and_functions_declarations/type_declaration.h"

//<"=" == move>
//<copy by function>

struct vector_struct
{
    //user must provide destructor function somewhere if he declares any vectors by MAKE_VEC macro
    void * vec;
    int size;
    int allocated_size;

    const TypePresenterContainer * type;
};
typedef struct vector_struct Vector;


//it will copy all values except first, which will be moved.
//if size is zero, def_value will be destroyed, BUT NOT freed.
//the best to make vector with zero size is call emptyVector().
//to make vector with some positive size, but not create elements, may provide def_value as NULL
//then it will be ignored. vector will calloc necessary space but will not fill it
Vector defaultVectorCalloc(int size, void * def_value, const TypePresenterContainer * type);
Vector * callocDefaultVector(int size, void * def_value, const TypePresenterContainer * type);

Vector defaultVectorWithStrictSize(int size, void * def_value, const TypePresenterContainer * type);


Vector emptyVector(const TypePresenterContainer * type);

Vector copyVector(const Vector * cop);
Vector* callocCopyVector(const Vector * cop);

Vector moveVector(Vector * mov);
static inline void * atVector(Vector * obj, int i);
static inline void VectorPopBack(Vector * vec)
{
    if (vec->size == 0)
        return;
    vec->type->destruct_function(atVector(vec, vec->size - 1));
    vec->size--;

}

void destructVector(Vector * obj);
void deleteVector(Vector ** obj);

static inline void * atVector(Vector * obj, int i)
{
    assert(obj->size > i);
    assert(i >= 0);
    return obj->vec + i * obj->type->element_size;
}
static inline const void * catVector(const Vector * obj, int i)
{
    assert(i < obj->size);
    assert(i >= 0);
    return obj->vec + i * obj->type->element_size;
}


static inline int VectorGetSize(const Vector * obj)
{
    return obj->size;
}
static inline bool VectorCanPushBack(const Vector * obj)
{
    return true;
}
//el will be moved not copied
void VectorPushBack(Vector * obj, void * el);

int VectorAsArrayIntGetSize(const Vector * obj);
bool VectorAsArrayIntCanPushBack(const Vector * obj);
void VectorAsArrayIntPushBack(Vector * obj, int el);



void VectorResize(Vector * vec, int new_size);

void freeBehind(void ** data);



//so MAKE_VEC assumes that type has been declared

#define MAKE_VEC(TN, UCN) struct Vector##UCN##_struct {                                 \
    Vector vec;                                                                         \
    TN * (*at)(struct Vector##UCN##_struct * obj, int i);                                   \
    const TN * (*cat)(const struct Vector##UCN##_struct * obj, int i);                     \
    int (*getSize)(const struct Vector##UCN##_struct * obj);                                \
    bool (*canPushBack)(const struct Vector##UCN##_struct * obj);                                   \
    void (*pushBack)(struct Vector##UCN##_struct * obj, TN el);                         \
    TN * (*back)(struct Vector##UCN##_struct * obj);                              \
    void (*pushBackCalloced)(struct Vector##UCN##_struct * obj, TN * el);                                                                     \
    void (*popBack)(struct Vector##UCN##_struct * obj);                                                                                                                \
    void (*resize)(struct Vector##UCN##_struct * obj, int new_size);\
} ;                                                                                                     \
typedef struct Vector##UCN##_struct Vector##UCN ;                                                                                                                      \
                                                                                                                                                                       \
 DECLARE_STRUCT_INLINE_TYPE(Vector##UCN, Vector##UCN)  ;                                                                                                 \
     /*guess i dont need it*/                                                                                                                                                        \
   static inline void* copyPtrVal##UCN(void * dest, const void * src, size_t n)                                                                                                       \
   {                                                                                                                                                                   \
        dest = calloc(1, n);                                                                                                                                      \
        *((TN *)dest) = copy##UCN((const TN*)src);                                                                                                                     \
        return NULL;                                            \
   }                                                                                                                \
                                                                                                                    \
                                                                                        \
                                                                                        \
static inline TN * atVector##UCN (Vector##UCN * obj, int i) {                                                   \
    return ( TN *)atVector(&obj->vec, i);                                                                                        \
}                                                                                                                               \
static inline const TN * catVector##UCN (const Vector##UCN * obj, int i)  {                                 \
    return (const TN *)catVector(&obj->vec, i);                                                                      \
}                                                                                                               \
static inline int vector##UCN##GetSize(const Vector##UCN * obj) {                                                           \
    return VectorGetSize(&obj->vec);                                                          \
}                                                                                                               \
static inline bool vector##UCN##CanPushBack(const Vector##UCN * obj) {                                                      \
    return VectorCanPushBack(&obj->vec);                                                          \
}                                                                                                                   \
static inline void vector##UCN##PushBack(Vector##UCN * obj, TN  el) {                                                                \
    VectorPushBack(&obj->vec,  (void *)&el )   ;                                                       \
}                                                                                       \
static inline TN * vector##UCN##Back(Vector##UCN * obj) {                   \
    return atVector(&obj->vec, obj->vec.size - 1);                                                                                        \
}                                                                                                                                             \
static inline void vector##UCN##PopBack(Vector##UCN * obj) {                                                                                  \
    VectorPopBack(&obj->vec);                                                                                                                                              \
}                                                                                                                                                                      \
static inline void vector##UCN##Resize(Vector##UCN * obj, int new_size) {                                                                                                            \
    VectorResize(&obj->vec,  new_size);                                                                                                                                                                       \
}\
static inline void vector##UCN##PushBackCalloced(Vector##UCN * obj, TN * el)            \
{                                                                                       \
    VectorPushBack(&obj->vec, (void *)el);                                           \
    free(el);                                                                                                \
}                                                                           \
                                                                                                                               \
static inline void assignFunctionsVector##UCN(Vector##UCN * obj) {                                                                  \
    obj->at = &atVector##UCN ;                                                                                      \
    obj->cat = &catVector##UCN ;                                                                                    \
    obj->getSize = &vector##UCN##GetSize;                                                                                   \
    obj->canPushBack = &vector##UCN##CanPushBack;                                                                               \
    obj->pushBack = &vector##UCN##PushBack;                                             \
    obj->back = &vector##UCN##Back;                                                     \
    obj->pushBackCalloced = &vector##UCN##PushBackCalloced;                                                                                   \
    obj->popBack = &vector##UCN##PopBack;                                                                                                                              \
    obj->resize = &vector##UCN##Resize;\
}                                                                                                                       \
static inline Vector##UCN defaultVector##UCN##Calloc(int size, TN def_value) {                                                              \
    Vector##UCN res;                                                                                        \
    res.vec = defaultVectorCalloc(size, (void *)&def_value, TYPE_##UCN());                                                    \
    assignFunctionsVector##UCN(&res);                                                                                                   \
    return res;                                                                                                     \
}                                                                                                                           \
static inline Vector##UCN * callocDefaultVector##UCN (int size, TN def_value) {                                                     \
    Vector##UCN * res = (Vector##UCN *)calloc(1, sizeof(Vector##UCN));                                                          \
    res->vec = defaultVectorCalloc(size, (void *)&def_value, TYPE_##UCN());                                                                   \
    assignFunctionsVector##UCN(res);                                                                                                \
    return res;                                                                                                                             \
}                                                                                       \
static inline Vector##UCN emptyVector##UCN() {                                          \
    Vector##UCN res;                                                                    \
    res.vec = emptyVector(TYPE_##UCN());                                                            \
    return res;\
}\
static inline Vector##UCN createVector##UCN##FromVector(Vector * vec) {                 \
    Vector##UCN res;                                                                    \
    res.vec = *vec;                                                                     \
    vec->vec = NULL;                                                                    \
    assignFunctionsVector##UCN(&res);                                                   \
    return res;\
}                                                                                       \
static inline Vector##UCN copyVector##UCN(const Vector##UCN * cop) {                    \
    Vector##UCN res;                                                                    \
    res.vec = copyVector(&cop->vec);                                                    \
    assignFunctionsVector##UCN(&res);                                               \
    return res;                                                                             \
}                                                                                        \
static inline Vector##UCN moveVector##UCN(Vector##UCN * mov)                                                                                                                 \
   {                                                                                                                                                                   \
    Vector##UCN res;                                                                    \
    res.vec = moveVector(&mov->vec);                                                    \
    assignFunctionsVector##UCN(&res);                                               \
    return res;                                               \
   }                                                                                       \
static inline Vector##UCN * callocVector##UCN##Copy(const Vector##UCN * cop) {          \
     Vector##UCN * res = (Vector##UCN *)calloc(1, sizeof(Vector##UCN));                 \
     *res = copyVector##UCN(cop);                                                       \
     return res;                                                                            \
}                                                                                        \
                                                                                        \
void destruct##UCN(TN * el);                                                                                                \
static inline void destructVector##UCN(Vector##UCN * obj) {                                                                                     \
    destructVector(&obj->vec);                                                                                                  \
}                                                                                                                               \
                                                                                                                            \
                                                                                                                                        \
static inline void deleteVector##UCN(Vector##UCN ** obj) {                                                                      \
    destructVector(&(*obj)->vec);                                                                                                               \
    free(*obj);                                                             \
 }                                                                                      \






#define at_1(X, Y) at((X), (Y))
#define at_2(X, Y, Z) at((X), (Y))->at((X)->at((X), (Y)), (Z))
#define at_3(X, Y, Z, T) at_2((X), (Y), (Z))->at((X)->at_2((X), (Y), (Z)), (T))
#define at_4(X, Y, Z, T, W) at_3((X), (Y), (Z), (T))->at((X)->at_3((X), (Y), (Z), (T)), (W))

#ifndef GREEKLATINGSQ_ARRAY2DINT_H
#define AT_SELECT(Q, W, E, R, T, macro, ...) macro
#define at(...) AT_SELECT(__VA_ARGS__, at_4, at_3, at_2, at_1, vd)(__VA_ARGS__)
#endif

#define cat_1(X, Y) cat((X), (Y))
#define cat_2(X, Y, Z) cat((X), (Y))->cat((X)->cat((X), (Y)), (Z))
#define cat_3(X, Y, Z, T) cat_2((X), (Y), (Z))->cat((X)->cat_2((X), (Y), (Z)), (T))
#define cat_4(X, Y, Z, T, W) cat_3((X), (Y), (Z), (T))->cat((X)->cat_3((X), (Y), (Z), (T)), (W))

#ifndef GREEKLATINGSQ_ARRAY2DINT_H
#define AT_SELECT(Q, W, E, R, T, macro, ...) macro
#define cat(...) AT_SELECT(__VA_ARGS__, cat_4, cat_3, cat_2, cat_1, vd)(__VA_ARGS__)
#endif

#define dim_1(X) getSize(X)
#define dim_2(X) (X)->cat((X), 0)->getSize((X)->cat((X), 0))
#define dim_3(X) (X)->cat((X), 0, 0)->getSize((X)->cat((X), 0, 0))
#define dim_4(X) (X)->cat((X), 0, 0, 0)->getSize((X)->cat((X), 0, 0, 0))

#define dim(X, Y) dim_##Y(X)








#endif //EXACT_COVER_Vector_H
