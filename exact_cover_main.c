#include "exact_cover/exactcover.h"
#include <stdio.h>
#include "user_interacts/user_interactions.h"
#include "vector/algorithms.h"


#define PRINT_SIZE
int main()
{
    setbuf(stdout, 0);
    int n, m;
    n = getValue(stdin);
    m = getValue(stdin);

    VectorVectorInt mat = defaultVectorVectorIntCalloc(n, defaultVectorIntCalloc(m, 0));
    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < m; ++j)
        {
            *mat.at(&mat, i, j) = getValue(stdin);
        }
    }

    VectorInt res = makeExactCover(&mat);
    if (res.getSize(&res) == 0)
        printf("NO COVER\n");
    else
    {
#ifdef PRINT_SIZE
        printf("%d ", res.getSize(&res));
#endif
        printfVectorInt("", &res);
    }

    destructVectorVectorInt(&mat);
    destructVectorInt(&res);

}


