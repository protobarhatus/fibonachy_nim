#include "user_interacts/user_interactions.h"
#include <stdio.h>
#include "greeklatinsquare/greeklatinsquare.h"
#include "vector/algorithms.h"
int main()
{
    GreekLatinSquare sq;
    fscanfGreekLatinSquare(stdin, &sq, SCANF_MODE_DIRECT);
    VectorVectorInt exact_cover = squareToExactCover(&sq);
    printf("%d %d\n", exact_cover.getSize(&exact_cover), dim(&exact_cover, 2));
    printfVectorVectorInt("", &exact_cover);
    return 0;
}

