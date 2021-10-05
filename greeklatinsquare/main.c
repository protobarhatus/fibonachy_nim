#include <stdio.h>
#include "permutations.h"
#include "greeklatinsquare.h"
#include <string.h>
#include <stdlib.h>

void test()
{
    FILE * in = stdin;
    int n;
    int i = 0;
    fscanf(in, "%d", &n);
    for (int i = 0; i < n; ++i)
    {
        GreekLatinSquare square;
        fscanfGreekLatinSquare(in, &square, SCANF_MODE_DIRECT);
        GreekLatinSquare ort = generateOrthogonal(&square, EXECUTION_LOG_MODE_NONE);
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


int main(int argc, char * argv[]) {

    setbuf(stdout, 0);
    ExecutionLogMode mode = chooseLogMode(argc, argv);
    if (argc > 1)
    {
        if (strcmp(argv[1], "test") == 0)
        {
            test();
            return 0;
        }
    }

    GreekLatinSquare square;
    //squares on scanf may be with any numbers > -n, but readen square will consist only of [1..n], and so orthogonal
    //if input square contains zero, it will be translated into n and other dig in range [1..n-1] will be without changing
    fscanfGreekLatinSquare(stdin, &square, SCANF_MODE_DIRECT);
    GreekLatinSquare ort = generateOrthogonal(&square, mode);


    if (!areOrthogonal(&square, &ort))
        printf("Error:\n");

    printfLatinSquare(&ort);
    return 0;
}
