#ifndef FIBONACHY_NIM_MATRIX_H
#define FIBONACHY_NIM_MATRIX_H
#include "../vector/vector.h"
#include "rational.h"

//in ideal  matrix should be on common type like Vector buuuuuut not now mb later
struct Matrix_struct {
#define Matrix struct Matrix_struct

    VectorVectorRational matrix;
    int lines, columns;
    VectorRational* (*at)(Matrix * a, int i);


#undef Matrix
};



typedef struct Matrix_struct Matrix;

Matrix defaultMatrix(int lines, int columns, Rational def_value);
Matrix makeUnitMatrix(int n);
Matrix destructMatrix(Matrix * mat);

static inline VectorRational * atMatrix(Matrix * mat, int i)
{
    return atVectorVectorRational(&mat->matrix, i);
}
static inline const VectorRational * catMatrix(const Matrix * mat, int i)
{
    return catVectorVectorRational(&mat->matrix, i);
}
static inline Rational * atMatrixEl(Matrix * mat, int i, int j)
{
    return mat->matrix.at(&mat->matrix, i, j);
}
static inline const Rational * catMatrixEl(const Matrix * mat, int i, int j)
{
    return mat->matrix.cat(&mat->matrix, i, j);
}

Matrix inverseMatrix(Matrix * matrix);

GENERATE_RIGHT_VALUE_UNARY_EXTENSIONS(Matrix, Matrix, inverse, Matrix);
void printfMatrix(const Matrix * mat);

#endif //FIBONACHY_NIM_MATRIX_H
