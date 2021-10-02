#ifndef GREEKLATINGSQ_GREEKLATINSQUARE_H
#define GREEKLATINGSQ_GREEKLATINSQUARE_H

#include <stdbool.h>
#include "Array2dInt.h"

//<"=" == copy>
//<no move>
struct GreekLatinSquare_struct
{
    Array2dInt square;
    int n;
};


typedef struct GreekLatinSquare_struct GreekLatinSquare;

GreekLatinSquare defaultGreekLatinSquare(int n);

GreekLatinSquare generateGreekLatinSquare(int n);

GreekLatinSquare generateOrthogonal(GreekLatinSquare * source);

int * atGreekLatinSquare(GreekLatinSquare * obj, int i, int j);

void printfGreekLatinSquare(GreekLatinSquare * obj);

enum ScanfMode
{
    SCANF_MODE_DIRECT,
    SCANF_MODE_DIVIDING
};


void scanfGreekLatinSquare(GreekLatinSquare * obj, enum ScanfMode mode);

bool areOrthogonal(GreekLatinSquare * a, GreekLatinSquare * b);


#endif //GREEKLATINGSQ_GREEKLATINSQUARE_H
