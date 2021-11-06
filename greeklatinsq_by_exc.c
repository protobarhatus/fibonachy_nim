#include <stdio.h>
#include "greeklatinsquare/greeklatinsquare.h"

#include <string.h>
#include "user_interacts/user_interactions.h"

void test()
{
    FILE * in = stdin;
    int n;
    n = getValue(in);
    for (int i = 0; i < n; ++i)
    {
        GreekLatinSquare square;
        fscanfGreekLatinSquare(in, &square, SCANF_MODE_DIRECT);
        GreekLatinSquare ort = generateOrthogonalByExactCover(&square);
        if (!areOrthogonal(&square, &ort))
        {
            printf("Error:\n");
            printfLatinSquare(&square);
            printf("------\n");
            printfLatinSquare(&ort);
            getchar();
        }
        printf("%d: success\n", i);
    }
    printf("Done\n");
}


int main(int argc, char * argv[])
{
    if (argc >= 2 && strcmp(argv[1], "-test") == 0)
    {
        test();
        return 0;
    }
    setbuf(stdout, 0);
    GreekLatinSquare square;
    //squares on scanf may be with any numbers > -n, but readen square will consist only of [1..n], and so orthogonal
    //if input square contains zero, it will be translated into n and other dig in range [1..n-1] will be without changing
    fscanfGreekLatinSquare(stdin, &square, SCANF_MODE_DIRECT);
    printf("-----\n");
    GreekLatinSquare ort = generateOrthogonalByExactCover(&square);
    printf("-----\n");
    if (isDefaultSquare(&ort))
    {
        printf("No orthogonal square exist\n");
        return 0;
    }
    if (!areOrthogonal(&square, &ort))
        printf("Error:\n");

    printfLatinSquare(&ort);
    return 0;

}
