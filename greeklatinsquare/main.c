#include <stdio.h>
#include "permutations.h"
#include "greeklatinsquare.h"

int main() {

    setbuf(stdout, 0);

    //GreekLatinSquare square = generateGreekLatinSquare(8);
    GreekLatinSquare  square = defaultGreekLatinSquare(10);

    scanfGreekLatinSquare(&square, SCANF_MODE_DIRECT);
    //printfGreekLatinSquare(&square);
    GreekLatinSquare ort = generateOrthogonal(&square);
    printfGreekLatinSquare(&ort);

    printf("ANSWER: %d", areOrthogonal(&square, &ort));
    return 0;
}
