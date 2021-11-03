#include "gen_lib/vector/vector.h"
#include "greeklatinsquare/greeklatinsquare.h"
#include "gen_lib/user_interacts/user_interactions.h"
#include <stdio.h>
#include "gen_lib/vector/algorithms.h"

//to use it, enter length of exact cover n and then n digits that resemble exact cover. n must be full square, its area of latin square
//and digits are order numbers of lines (starting from 0) in exact cover that was generated exactly in format of translation greek
//lating squares to exact cover, i.e. orig exact cover has n^(3/2) - n + n^(1/2) lines.
//Input values must be sorted by increasing.
//consistent execution of square_to_exact_cover and exact_cover_solve must give right form of exact_cover
int main()
{
    printf("Enter exact cover length first: ");
    int n = getValue(stdin);
    VectorInt cover = defaultVectorIntCalloc(n, 0);
    scanfVectorInt(n, &cover);
    GreekLatinSquare square = exactCoverToSquare(&cover);
    printfLatinSquare(&square);

    destructVectorInt(&cover);

}

