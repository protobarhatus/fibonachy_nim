#include <stdio.h>
#include "greeklatinsquare/greeklatinsquare.h"
#include "gen_lib/user_interacts/user_interactions.h"

int main(int argc, char * argv[])
{
    int n, k;
    scanf("%d%d", &n, &k);
    GreekLatinSquare square = generateLatinSquare(n, k);
    printfLatinSquare(&square);
}