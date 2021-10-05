#ifndef GREEKLATINGSQ_GREEKLATINSQUARE_H
#define GREEKLATINGSQ_GREEKLATINSQUARE_H

#include <stdbool.h>
#include "Array2dInt.h"
#include <stdio.h>
#include "user_interactions.h"

//<"=" == copy>
//<no move>
struct GreekLatinSquare_struct
{
    Array2dInt square;
    int n;
};


typedef struct GreekLatinSquare_struct GreekLatinSquare;

GreekLatinSquare defaultGreekLatinSquare(int n);

GreekLatinSquare generateLatinSquare(int n, int ans_required_number, ExecutionLogMode mode);

GreekLatinSquare generateOrthogonal(GreekLatinSquare * source, ExecutionLogMode mode);

static inline int * atGreekLatinSquare(GreekLatinSquare * obj, int i, int j)
{
    return atArray2dInt(&obj->square, i, j);
}

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
