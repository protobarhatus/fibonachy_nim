#include <stdio.h>
#include "vectordef.h"
#include "stdlib.h"
#include "string.h"



static void locateMem(Vector * obj, int amount_of_el, int el_size)
{
    obj->size = amount_of_el;
    if (amount_of_el == 0)
        obj->allocated_size = 8;
    obj->allocated_size = 2 * obj->size;
    obj->vec = (void *)calloc(obj->allocated_size, el_size);

}

Vector defaultVectorCalloc(int size, void * def_value, const TypePresenterContainer * type)
{
    Vector obj;
    locateMem(&obj, size, type->element_size);
    if (size != 0 && def_value != NULL)
    {
        for (int i = 1; i < obj.size; ++i)
            type->cpy_func(obj.vec + i * type->element_size, def_value, type->element_size);
        type->move_function(obj.vec, def_value, type->element_size);
    }
    else
    {
        if (def_value != NULL)
            type->destruct_function(def_value);
    }

    obj.type = type;

    return obj;
}

Vector * callocDefaultVector(int size, void * def_value, const TypePresenterContainer * type)
{
    Vector *obj = (Vector*)calloc(1, sizeof(Vector));
    locateMem(obj, size, type->element_size);
    if (size != 0 && def_value != NULL)
    {
        for (int i = 1; i < obj->allocated_size; ++i)
            type->cpy_func(obj->vec + i * type->element_size, def_value, type->element_size);
        type->move_function(obj->vec, def_value, type->element_size);
    }
    else
        free(def_value);

    obj->type = type;
    return obj;
}

Vector defaultVectorWithStrictSize(int size, void * def_value, const TypePresenterContainer * type)
{
    Vector obj;
    obj.size = size;
    obj.allocated_size = obj.size;
    obj.vec = (void *)calloc(obj.allocated_size, type->element_size);


    if (size != 0 && def_value != NULL)
    {
        for (int i = 1; i < obj.allocated_size; ++i)
            type->cpy_func(obj.vec + i * type->element_size, def_value, type->element_size);
        type->move_function(obj.vec, def_value, type->element_size);
    }
    else
        free(def_value);

    obj.type = type;

    return obj;
}

Vector copyVector(const Vector * cop)
{
    Vector res;
    if (cop->allocated_size > 0)
        res.vec = calloc(cop->allocated_size, cop->type->element_size);
    for (int i = 0; i < cop->size; ++i)
        cop->type->cpy_func(res.vec + i * cop->type->element_size, cop->vec + i * cop->type->element_size, cop->type->element_size);
    res.size = cop->size;
    res.allocated_size = cop->allocated_size;


    res.type = cop->type;
    return res;
}

Vector moveVector(Vector * cop)
{
    Vector res;
    res.vec = cop->vec;
    cop->vec = NULL;
    res.size = cop->size;
    res.allocated_size = cop->allocated_size;


    res.type = cop->type;
    return res;
}
Vector* callocCopyVector(const Vector * cop)
{
    Vector * res = calloc(1, sizeof(Vector));
    *res = copyVector(cop);
    return res;

}



void VectorPushBack(Vector * obj, void * el)
{
    assert(VectorCanPushBack(obj));
    if (obj->size < obj->allocated_size)
    {
        obj->type->move_function(obj->vec + obj->size * obj->type->element_size, el, obj->type->element_size);
        ++obj->size;
        return;
    }

    obj->allocated_size *= 2;
    obj->allocated_size += 1;
    void * call_res = realloc(obj->vec, obj->allocated_size * obj->type->element_size);
    assert(call_res != NULL);
    if (call_res == obj->vec)
    {
        obj->type->move_function(obj->vec + obj->size * obj->type->element_size, el, obj->type->element_size);
        ++obj->size;
        return;
    }
    else
    {
        obj->vec = call_res;

        obj->type->move_function(obj->vec + obj->size * obj->type->element_size, el, obj->type->element_size);
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



void freeBehind(void ** data)
{
    free(*data);
}

void destructVector(Vector * obj)
{
    if (obj->vec != NULL)
    {
        for (int i = 0; i < obj->size; ++i)
            obj->type->destruct_function(obj->vec + i * obj->type->element_size);
        free(obj->vec);
    }
    obj->vec = NULL;

}

void deleteVector(Vector ** obj)
{
    destructVector(*obj);
    free(*obj);
}

Vector emptyVector(const TypePresenterContainer * type)
{
    Vector res;
    res.size = 0;
    res.allocated_size = 0;
    res.type = type;
    res.vec = NULL;
    return res;
}



void VectorResize(Vector * vec, int new_size)
{
    if (vec->allocated_size < new_size)
    {
        while (vec->allocated_size < new_size)
        {
            vec->allocated_size += 1;
            vec->allocated_size *= 2;
        }
        vec->vec = realloc(vec, vec->allocated_size*sizeof(int));
    }
    for (int i = vec->size - 1; i >= new_size; --i)
        vec->type->destruct_function(atVector(vec, i));
    vec->size = new_size;
}


