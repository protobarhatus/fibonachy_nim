#include <stdio.h>
#include "matrix.h"
#include "../vector/algorithms.h"

void assignFunctions(Matrix * mat)
{
    mat->at = &atMatrix;
}

Matrix defaultMatrix(int lines, int columns, Rational def_value)
{
    Matrix result;
    result.lines = lines;
    result.columns = columns;

    result.matrix = defaultVectorVectorRationalCalloc(lines, defaultVectorRationalCalloc(columns, def_value));
    assignFunctions(&result);
    return result;
}

Matrix makeUnitMatrix(int n)
{
    Matrix res;
    res.lines = n;
    res.columns = n;
    res.matrix = defaultVectorVectorRationalCalloc(n, defaultVectorRationalCalloc(n, nullRational()));
    for (int i = 0; i < n; ++i)
        *res.matrix.at(&res.matrix, i, i) = unitRational();
    assignFunctions(&res);
    return res;
}
Matrix destructMatrix(Matrix * mat)
{
    destructVectorVectorRational(&mat->matrix);
}

static int getIndexOfFirstNonZero(VectorRational * vec)
{
    for (int i = 0; i < vec->getSize(vec); ++i)
        if (!isNull(vec->at(vec, i)) )
            return i;
    return vec->getSize(vec);
}

static int pairSecComparator(const void * ap, const void * bp)
{
    PairInt * a = (PairInt*)ap;
    PairInt * b = (PairInt*)bp;
    if (a->second < b->second)
        return -1;
    if (a->second > b->second)
        return 1;
    //first does not matter
    return 0;
}

static void swapVectorsRat(VectorRational * a, VectorRational * b)
{
    VectorRational t;
    memcpy(&t, a, sizeof(VectorRational));
    memcpy(a, b, sizeof(VectorRational));
    memcpy(b, &t, sizeof(VectorRational));
}

static void sortRows(int height, Matrix * matrix, VectorPairInt * indexes_of_first_non_zero, Matrix * result)
{
    for (int i = 0; i < height; ++i)
    {
        *indexes_of_first_non_zero->at(indexes_of_first_non_zero, i) =
                defaultPairInt( i, getIndexOfFirstNonZero(matrix->matrix.at(&matrix->matrix, i)) );
    }
    sortVectorPairInt(indexes_of_first_non_zero, &pairSecComparator);
    for (int i = 0; i < height; ++i)
    {
        if (i < atVectorPairInt(indexes_of_first_non_zero, i)->first)
        {
            swapVectorsRat(atMatrix(matrix, i), atMatrix(matrix, atVectorPairInt(indexes_of_first_non_zero, i)->first));
            swapVectorsRat(atMatrix(result, i), atMatrix(result, atVectorPairInt(indexes_of_first_non_zero, i)->first));
        }
    }
}

static void divideOnLeadElement(Matrix * matrix, Matrix * result, int x)
{
    Rational lead_el = *atMatrixEl(matrix, x, x);
    for (int i = x; i < matrix->lines; ++i)
    {
        divToRational(atMatrixEl(matrix, x, i), &lead_el);
    }
    for (int i = 0; i < matrix->lines; ++i)
    {
        divToRational(atMatrixEl(result, x, i), &lead_el);
    }
}

//by gauss algorithm
//algorithm will give divide by zero error somewhere (in divideByLeadElement) if determinant is null
Matrix inverseMatrix(Matrix * matrix)
{
//only squares matrix are inverted
assert(matrix->lines == matrix->columns);
    Matrix result = makeUnitMatrix(matrix->lines);
    int height = matrix->lines;
    int width = matrix->columns;

    VectorPairInt indexes_of_first_non_zero = defaultVectorPairIntCalloc(height, defaultPairInt(0,0));


    sortRows(height, matrix, &indexes_of_first_non_zero, &result);
//to triangular form
    for (int n = 0; n < height; ++n) {
        divideOnLeadElement(matrix, &result, n);
        for (int i = n + 1; i < height; ++i)
        {
            Rational multiplier = minusRationalRV(divRational(atMatrixEl(matrix, i, n), atMatrixEl(matrix, n, n)));
            //HOW ITS UGLY IN C
            replaceVectorRational(atMatrix(matrix, i) , addVectorRationalLVRV(matrix->at(matrix, i),
                                                         vectorRationalMultedOnNum(matrix->at(matrix, n), &multiplier)));
            replaceVectorRational(atMatrix(&result, i) , addVectorRationalLVRV(atMatrix(&result, i),
                                                          vectorRationalMultedOnNum(atMatrix(&result, n) , &multiplier)));


        }

    }

//back to normal
    for (int n = height - 1; n >= 0; --n)
    {
        for (int i = n - 1; i >= 0; --i)
        {
            Rational multiplier = minusRationalRV(divRational(matrix->at(matrix, i, n), matrix->at(matrix, n, n)));
            replaceVectorRational(atMatrix(matrix, i) , addVectorRationalLVRV(matrix->at(matrix, i),
                                                         vectorRationalMultedOnNum(matrix->at(matrix, n), &multiplier)));
            replaceVectorRational(atMatrix(&result, i) , addVectorRationalLVRV(atMatrix(&result, i),
                                                          vectorRationalMultedOnNum(atMatrix(&result, n) , &multiplier)));
        }
    }
    destructVectorPairInt(&indexes_of_first_non_zero);
    return result;
}

void printfMatrix(const Matrix * mat)
{
    for (int i = 0; i < mat->lines; ++i)
    {
        for (int j = 0; j < mat->columns; ++j)
        {
            printf("%lld/%lld    ", catMatrixEl(mat, i, j)->num, catMatrixEl(mat, i, j)->denom);
        }
        printf("\n");
    }
    printf("\n");
}