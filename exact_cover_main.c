#include "exact_cover/exactcover.h"
#include "vector/vector.h"
#include <stdio.h>
#include "user_interacts/user_interactions.h"
#include "vector/algorithms.h"

int main()
{
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
    printfVectorInt("", &res);


}


