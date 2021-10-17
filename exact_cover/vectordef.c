#include <stdio.h>
#include "vectordef.h"
#include "stdlib.h"
#include "string.h"
void assignFunctions(vector * obj)
{
    obj->at = &atVector;
    obj->cat = &catVector;
    obj->getSize = &VectorGetSize;
    obj->canPushBack = &VectorCanPushBack;
    obj->pushBack = &VectorPushBack;
}

void assignFunctionsAsArrayInt(vector * obj)
{
    obj->at = &atVectorAsArrayInt;
    obj->cat = &catVectorAsArrayInt;
    obj->getSize = &VectorAsArrayIntGetSize;
    obj->canPushBack = &VectorAsArrayIntCanPushBack;
    obj->pushBack = &VectorAsArrayIntPushBack;
}

void nonConstMemcpyDecl(void * dest, void * src, size_t n)
{
    memcpy(dest, (const void *)src, n);
}

static void locateMem(vector * obj, int amount_of_el, int el_size)
{
    obj->size = amount_of_el;
    if (amount_of_el == 0)
        obj->allocated_size = 8;
    obj->allocated_size = 2 * obj->size;
    obj->vec = (void *)calloc(obj->allocated_size, el_size);
    obj->element_size = el_size;
}

vector defaultVectorCalloc(int size, void * def_value, int el_size, CopyFunction cpy_func, DestructFunction destr, MoveFunction move_f)
{
    vector obj;
    assignFunctions(&obj);
    locateMem(&obj, size, el_size);
    if (size != 0)
    {
        for (int i = 1; i < obj.allocated_size; ++i)
            cpy_func(obj.vec + i * el_size, def_value, el_size);
        move_f(obj.vec, def_value, el_size);
    }
    obj.stack_version = NULL;
    obj.cpy_func = cpy_func;
    obj.destruct_function = destr;
    obj.move_function = move_f;

    return obj;
}

vector * callocDefaultVector(int size, void * def_value, int el_size, CopyFunction cpy_func, DestructFunction destr, MoveFunction move_func)
{
    vector *obj = (vector*)calloc(1, sizeof(vector));
    assignFunctions(obj);
    locateMem(obj, size, el_size);
    if (size != 0)
    {
        for (int i = 1; i < obj->allocated_size; ++i)
            cpy_func(obj->vec + i * el_size, def_value, el_size);
        move_func(obj->vec, def_value, el_size);
    }
    obj->stack_version = NULL;
    obj->cpy_func = cpy_func;
    obj->destruct_function = destr;
    obj->move_function = move_func;
    return obj;
}

vector copyVector(const vector * cop)
{
    vector res;
    res.vec = calloc(cop->allocated_size, cop->element_size);
    for (int i = 0; i < cop->size; ++i)
        cop->cpy_func(res.vec + i * cop->element_size, cop->vec + i * cop->element_size, cop->element_size);
    res.size = cop->size;
    res.allocated_size = cop->allocated_size;
    res.stack_version = cop->stack_version;
    res.element_size = cop->element_size;

    res.at = cop->at;
    res.cat = cop->cat;
    res.getSize = cop->getSize;
    res.canPushBack = cop->canPushBack;
    res.pushBack = cop->pushBack;

    res.cpy_func = cop->cpy_func;
    res.destruct_function = cop->destruct_function;
    res.move_function = cop->move_function;
    return res;
}

vector moveVector(vector * cop)
{
    vector res;
    res.vec = cop->vec;
    cop->vec = NULL;
    res.size = cop->size;
    res.allocated_size = cop->allocated_size;
    res.stack_version = cop->stack_version;
    res.element_size = cop->element_size;

    res.at = cop->at;
    res.cat = cop->cat;
    res.getSize = cop->getSize;
    res.canPushBack = cop->canPushBack;
    res.pushBack = cop->pushBack;

    res.cpy_func = cop->cpy_func;
    res.destruct_function = cop->destruct_function;
    res.move_function = cop->move_function;
    return res;
}
vector* callocCopyVector(const vector * cop)
{
    vector * res = calloc(1, sizeof(vector));
    *res = copyVector(cop);
    return res;

}

int VectorGetSize(const vector * obj)
{
    return obj->size;
}
bool VectorCanPushBack(const vector * obj)
{
    return true;
}
void VectorPushBack(vector * obj, void * el)
{
    assert(obj->canPushBack(obj));
    if (obj->size < obj->allocated_size)
    {
        obj->move_function(obj->vec + obj->size * obj->element_size, el, obj->element_size);
        ++obj->size;
        return;
    }

    obj->allocated_size *= 2;
    obj->allocated_size += 1;
    void * call_res = realloc(obj->vec, obj->allocated_size * obj->element_size);
    assert(call_res != NULL);
    if (call_res == obj->vec)
    {
        obj->move_function(obj->vec + obj->size * obj->element_size, el, obj->element_size);
        ++obj->size;
        return;
    }
    else
    {
        obj->vec = call_res;

        obj->move_function(obj->vec + obj->size * obj->element_size, el, obj->element_size);
        ++obj->size;
        return;
    }

 /*   void * new_vec = (void *)calloc(obj->allocated_size, obj->element_size);
    assert(new_vec != NULL);
    for (int i = 0; i < obj->size; ++i)
        obj->move_function(new_vec + i * obj->element_size, obj->vec + i * obj->element_size, obj->element_size);
    free(obj->vec);
    obj->vec = new_vec;

    obj->move_function(obj->vec + obj->size * obj->element_size, el, obj->element_size);
    ++obj->size;*/
}

int VectorAsArrayIntGetSize(const vector * obj)
{
    assert(obj->stack_version != NULL);
    return obj->stack_version->getSize(obj->stack_version);
}
bool VectorAsArrayIntCanPushBack(const vector * obj)
{
    assert(obj->stack_version != NULL);
    return obj->stack_version->canPushBack(obj->stack_version);
}
void VectorAsArrayIntPushBack(vector * obj, int el)
{
    assert(obj->stack_version != NULL);
    return obj->stack_version->pushBack(obj->stack_version, el);
}

void freeBehind(void ** data)
{
    free(*data);
}

void destructVector(vector * obj)
{
    if (obj->vec != NULL)
    {
        for (int i = 0; i < obj->size; ++i)
            obj->destruct_function(obj->vec + i * obj->element_size);
        free(obj->vec);
    }
    obj->vec = NULL;
    obj->stack_version = NULL;
}

void deleteVector(vector ** obj)
{
    destructVector(*obj);
    free(*obj);
}

vector VectorAsArrayWrap(ArrayInt * arr)
{
    vector res;
    assignFunctionsAsArrayInt(&res);
    res.stack_version = arr;
    res.size = arr->getSize(arr);
    res.vec = NULL;
    res.allocated_size = -1;

    res.cpy_func = &memcpy;
    res.destruct_function = &dummyDestructor;

    return res;
}
vector * callocVectorAsArrayWrap(ArrayInt * arr)
{
    vector * res = (vector *)calloc(1, sizeof(vector));
    assignFunctionsAsArrayInt(res);
    res->stack_version = arr;
    res->size = arr->getSize(arr);
    res->vec = NULL;
    res->allocated_size = -1;

    res->cpy_func = &memcpy;
    res->destruct_function = &dummyDestructor;
    return res;
}

void dummyDestructor(void * r)
{

}




void VectorPopBack(vector * vec)
{
    if (vec->size == 0)
        return;
    vec->destruct_function(vec->at(vec, vec->size - 1));
    vec->size--;

}

