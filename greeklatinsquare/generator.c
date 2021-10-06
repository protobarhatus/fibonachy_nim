#include <stdio.h>
#include "greeklatinsquare.h"
#include "user_interactions.h"

int main(int argc, char * argv[])
{
    int n, k;
    scanf("%d%d", &n, &k);
    GreekLatinSquare square = generateLatinSquare(n, k, chooseLogMode(argc, argv));
    printfLatinSquare(&square);
}