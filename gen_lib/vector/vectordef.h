#ifndef EXACT_COVER_Vector_H
#define EXACT_COVER_Vector_H

#include <stddef.h>
#include "arrayint.h"
#include <stdlib.h>
#include <string.h>


typedef void * (*CopyFunction)(void * dest, const void * src, size_t n);
typedef void * (*MoveFunction)(void * dest, void * src, size_t n);
typedef void (*DestructFunction)(void * el);

//<"=" == move>
//<copy by function>
//usually i make structures in UpperCamelCase, but now its exception cause std::vector and cause Vector may conflict with geometrical thinq

struct vector_struct
{
    //user must provide destructor function somewhere if he declares any vectors by MAKE_VEC macro
    void * vec;
    int size;
    int allocated_size;
    int element_size;
    //vector does not own it and doesn't response for callocing and deleting it
    ArrayInt * stack_version;

    CopyFunction cpy_func;
    DestructFunction destruct_function;
    MoveFunction move_function;
};
typedef struct vector_struct vector;

//In initizialization and pushBack It will destruct data that is provided in arguments, so provide only as rvalue
//but it will not move it but copy
vector defaultVectorCalloc(int size, void * def_value, int el_size, CopyFunction cpy_func, DestructFunction destr_f, MoveFunction move_f);
vector * callocDefaultVector(int size, void * def_value, int el_size, CopyFunction cpy_func, DestructFunction destr_f, MoveFunction move_f);

vector VectorAsArrayWrap(ArrayInt * arr);
vector * callocVectorAsArrayWrap(ArrayInt * arr);

vector copyVector(const vector * cop);
vector* callocCopyVector(const vector * cop);

vector moveVector(vector * mov);
static inline void * atVector(vector * obj, int i);
static inline void VectorPopBack(vector * vec)
{
    if (vec->size == 0)
        return;
    vec->destruct_function(atVector(vec, vec->size - 1));
    vec->size--;

}

void destructVector(vector * obj);
void deleteVector(vector ** obj);

static inline void * atVector(vector * obj, int i)
{
    assert(obj->size > i);
    assert(i >= 0);
    return obj->vec + i * obj->element_size;
}
static inline const void * catVector(const vector * obj, int i)
{
    assert(i < obj->size);
    assert(i >= 0);
    return obj->vec + i * obj->element_size;
}

static inline void * atVectorAsArrayInt(vector * obj, int i)
{
    assert(obj->stack_version != NULL);
    return obj->stack_version->at(obj->stack_version, i);
}

static inline const void * catVectorAsArrayInt(const vector * obj, int i)
{
    assert(obj->stack_version != NULL);
    return obj->stack_version->cat(obj->stack_version, i);
}

static inline int VectorGetSize(const vector * obj)
{
    return obj->size;
}
static inline bool VectorCanPushBack(const vector * obj)
{
    return true;
}
void VectorPushBack(vector * obj, void * el);

int VectorAsArrayIntGetSize(const vector * obj);
bool VectorAsArrayIntCanPushBack(const vector * obj);
void VectorAsArrayIntPushBack(vector * obj, int el);

void dummyDestructor(void * );

void VectorResize(vector * vec, int new_size);

void freeBehind(void ** data);

void nonConstMemcpyDecl(void * dest, void * src, size_t n);

//If type TN is SIMPLE then it means that copy is completed by memcpy and destruction is unneded cause it don't need
//to free some data inside
//but functions copy##UCN and destruct##UCN still have to exist
//otherwise,
//User must provide functions copy##UCN and destruct##UCN except if TN is Vector itself
//then if initialized in right order, that functions will be auto generated
//if its pointer on simple type then destroying is just calling free().
//pointers on non simple types must provide appropriate cpyFunc and destrFunc
//cpyFunc means FULL copy, (with copy of data behind inside pointers)
#define SIMPLE_TYPE &memcpy, &dummyDestructor, &nonConstMemcpyDecl
#define POINTER_TYPE(UCN) &copyPtrVal##UCN, &freeBehind, &nonConstMemcpyDecl
#define STRUCT_TYPE(UCN) &cpyFunc##UCN, &destrFunc##UCN, &moveFunc##UCN

#define MAKE_VEC(TN, UCN, ___COPY_FUNCTION_PTR___, ___DESTRUCT_FUNCTION_PTR___, ___MOVE_FUNCTION_PTR___) struct Vector##UCN##_struct {                                 \
    vector vec;                                                                         \
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
typedef struct Vector##UCN##_struct Vector##UCN ;                                                               \
                                                                                                    \
   TN copy##UCN(const TN * src);                                                           \
   /*return value absolutely doesn't matter but it need to be the same type with memcpy*/                                                                                                 \
   static inline void* cpyFunc##UCN(void * dest, const void * src, size_t n)                                \
   {                                                                                                    \
        *((TN *)dest) = copy##UCN((const TN*)src);                                                              \
        return NULL;                                                                                        \
   }                                                                                    \
                                                                                        \
     void destruct##UCN(TN * el);                                                                                   \
    static inline void destrFunc##UCN(void * el)                                        \
    {                                                                                   \
         destruct##UCN((TN*)el);                                                                               \
    }                                                                                                                                                                  \
     TN move##UCN(TN * el);                                                                                                                                          \
      static inline void* moveFunc##UCN(void * dest, void * src, size_t n)                                                                                              \
      {                                                                                                                                                                \
           *((TN *)dest) = move##UCN((TN*)src);                                                                                                                        \
           return NULL;                                                             \
      }                                                                                                                                                                \
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
/* it STILL makes copy (full copy), BUT it frees data from pointer */                                                                                                            \
static inline void vector##UCN##PushBackCalloced(Vector##UCN * obj, TN * el)            \
{                                                                                       \
    VectorPushBack(&obj->vec, (void *)el);                                           \
    free(el);                                                                                                \
}                                                                           \
/* in this kind of wrapping, there is only one correct combination of functions and so all it copy and assign work is concentrated only here*/                                                                                                                                \
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
    res.vec = defaultVectorCalloc(size, (void *)&def_value, sizeof(TN), ___COPY_FUNCTION_PTR___, ___DESTRUCT_FUNCTION_PTR___, ___MOVE_FUNCTION_PTR___);                                                    \
    assignFunctionsVector##UCN(&res);                                                                                                   \
    return res;                                                                                                     \
}                                                                                                                           \
static inline Vector##UCN * callocDefaultVector##UCN (int size, TN def_value) {                                                     \
    Vector##UCN * res = (Vector##UCN *)calloc(1, sizeof(Vector##UCN));                                                          \
    res->vec = defaultVectorCalloc(size, (void *)&def_value, sizeof(TN), ___COPY_FUNCTION_PTR___, ___DESTRUCT_FUNCTION_PTR___, ___MOVE_FUNCTION_PTR___);                                                                   \
    assignFunctionsVector##UCN(res);                                                                                                \
    return res;                                                                                                                             \
}                                                                                       \
static inline Vector##UCN vector##UCN##AsArrayWrap(ArrayInt * arr)                      \
{                                                                                       \
         Vector##UCN res;                                                               \
         res.vec = VectorAsArrayWrap(arr);                                              \
         assignFunctionsVector##UCN(&res);                                                \
         return res;                                                                    \
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
