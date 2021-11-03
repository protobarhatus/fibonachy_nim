#ifndef GREEKLATINGSQ_GREEKLATINSQUARE_H
#define GREEKLATINGSQ_GREEKLATINSQUARE_H

#include <stdbool.h>
#include "../gen_lib/vector/Array2dInt.h"
#include "../gen_lib/vector/vector.h"
#include <stdio.h>

//<"=" == copy>
//<no move>
struct GreekLatinSquare_struct
{
    Array2dInt square;
    int n;
};


typedef struct GreekLatinSquare_struct GreekLatinSquare;

GreekLatinSquare defaultGreekLatinSquare(int n);

GreekLatinSquare generateLatinSquare(int n, int ans_required_number);

GreekLatinSquare generateOrthogonalByBacktracking(GreekLatinSquare * source);

VectorVectorInt squareToExactCover(GreekLatinSquare * source);

GreekLatinSquare exactCoverToSquare(VectorInt * cover);

GreekLatinSquare generateOrthogonalByExactCover(GreekLatinSquare * source);

int * atGreekLatinSquare(GreekLatinSquare * obj, int i, int j);

void printfLatinSquare(GreekLatinSquare * obj);

void printfGreekLatinSquare(GreekLatinSquare * a, GreekLatinSquare * b);

enum ScanfMode
{
    SCANF_MODE_DIRECT,
    SCANF_MODE_DIVIDING
};


void fscanfGreekLatinSquare(FILE * file, GreekLatinSquare * obj, enum ScanfMode mode);

bool areOrthogonal(GreekLatinSquare * a, GreekLatinSquare * b);

bool isLatin(GreekLatinSquare * square);

bool isDefaultSquare(GreekLatinSquare * square);
#endif //GREEKLATINGSQ_GREEKLATINSQUARE_H
