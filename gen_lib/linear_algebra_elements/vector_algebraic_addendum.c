
#include "vector_algebraic_addendum.h"
#include "../vector/algorithms.h"

void addToVector(Vector * dest, const Vector * b, const ArithmeticTypePresenter * artype)
{
    //we can compare pointers by value because there is only one instance of type presenter
    assert(dest->size == b->size);
    assert(areSameTypes(dest->type, b->type));

    for (int i = 0; i < dest->size; ++i)
        artype->addTo((void*) atVector(dest, i), (const void*) catVector(b, i));

}

void subToVector(Vector * dest, const Vector * b, const ArithmeticTypePresenter * artype)
{
    assert(dest->size == b->size);
    assert(areSameTypes(dest->type , b->type));

    for (int i = 0; i < dest->size; ++i)
        artype->subTo((void*) atVector(dest, i), (const void*) catVector(b, i));
}

Vector addVector(const Vector * a, const Vector * b, const ArithmeticTypePresenter * artype)
{
    assert(a->size == b->size);
    assert(areSameTypes(a->type , b->type));

    Vector res = defaultVectorWithStrictSize(a->size, NULL, a->type);
    for (int i = 0; i < a->size; ++i)
        artype->add(atVector(&res, i), catVector(a, i), catVector(b, i));
    return res;
}

Vector subVector(const Vector * a, const Vector * b, const ArithmeticTypePresenter * artype)
{
    assert(a->size == b->size);
    assert(areSameTypes(a->type , b->type));

    Vector res = defaultVectorWithStrictSize(a->size, NULL, a->type);
    for (int i = 0; i < a->size; ++i)
        artype->sub(atVector(&res, i), catVector(a, i), catVector(b, i));
    return res;
}
//el такого же типа как элементы вектора, но проверять это здесь нет возможности
void multVectorOnNum(Vector * dest, const void * el,  const ArithmeticTypePresenter * artype)
{
    for (int i = 0; i < dest->size; ++i)
        artype->multTo(atVector(dest, i), el);
}
Vector vectorMultedOnNum(const Vector * vec, const void * el, const  ArithmeticTypePresenter * artype)
{
    Vector res = defaultVectorWithStrictSize(vec->size, NULL, vec->type);
    for (int i = 0; i < vec->size; ++i)
        artype->mult(atVector(&res, i), catVector(vec, i), el);
    return res;
}

void * scalar(const Vector * a, const Vector * b, const ArithmeticTypePresenter * artype)
{
    assert(a->size == b->size);
    assert(areSameTypes(a->type , b->type));

    void * res = artype->callocZeroValue();
    void * mult_buffer = malloc(a->type->element_size);
    for (int i = 0; i < a->size; ++i)
    {
        artype->mult(mult_buffer, catVector(a, i), catVector(b, i));
        artype->addTo(res, mult_buffer);
    }

    free(mult_buffer);

    return res;
}

void * lengthVector(const Vector * a, const ArithmeticTypePresenter * artype)
{
    void * res = artype->callocZeroValue();
    void * mult_buff = malloc(a->type->element_size);
    for (int i = 0; i < a->size; ++i)
    {
        artype->mult(mult_buff, catVector(a, i), catVector(a, i));
        artype->addTo(res, mult_buff);
    }

    free(mult_buff);
    artype->sqrt(res, res);
    return res;
}
void * distanceVector(const Vector * a, const Vector * b, const ArithmeticTypePresenter * artype)
{
    assert(a->size == b->size);
    assert(areSameTypes(a->type , b->type));
    void * res = artype->callocZeroValue();
    void * buff = malloc(a->type->element_size);

    for (int i = 0; i < a->size; ++i)
    {
        artype->sub(buff, catVector(a, i), catVector(b, i));
        artype->mult(buff, buff, buff);
        artype->addTo(res, buff);
    }

    free(buff);
    return res;
}