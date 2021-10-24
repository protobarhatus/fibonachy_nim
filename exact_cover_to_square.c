#include "vector/vector.h"
#include "greeklatinsquare/greeklatinsquare.h"
#include "user_interacts/user_interactions.h"
#include <stdio.h>
#include "vector/algorithms.h"

int main()
{
    printf("Enter exact cover length first: ");
    int n = getValue(stdin);
    VectorInt cover = defaultVectorIntCalloc(n, 0);
    scanfVectorInt(n, &cover);
    GreekLatinSquare square = exactCoverToSquare(&cover);
    printfLatinSquare(&square);
}

