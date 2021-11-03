#include "algorithms.h"
#include "stdio.h"

void printfVectorInt(const char * premessage, VectorInt * vec)
{
    printf("%s", premessage);
    int size = vec->getSize(vec);
    for (int i = 0; i < size; ++i)
        printf("%d ", *vec->at(vec, i));
    printf("\n");
}

void printfVectorVectorInt(const char * premessage, VectorVectorInt * vec)
{
    printf("%s", premessage);
    int size = vec->getSize(vec);
    for (int i = 0; i < size; ++i)
        printfVectorInt("", atVectorVectorInt(vec, i));
    printf("\n");
}

int intLessComparator(const void * a, const void * b)
{
    const int * a_p = (const int*)a;
    const int * b_p = (const int*)b;
    return ((*a_p) == (*b_p) ? 0 : ((*a_p) > (*b_p) ? 1 : -1));
}

void scanfVectorInt(int size, VectorInt * vec)
{
    for (int i = 0; i < size; ++i)
        scanf("%d", vec->at(vec, i));

}
void scanfVectorVectorInt(int lin, int col, VectorVectorInt * vec)
{
    for (int i = 0; i < lin; ++i)
    {
        scanfVectorInt(col, vec->at(vec, i));
    }
}
static long long int iabs(long long int a)
{
    return a > 0 ? a : -a;
}
long long int eu_mod(long long int x, long long int y)
{
    long long int r;
    assert(y != 0);
    r = x % y;
    if (r < 0)
        r += iabs(y);
    return r;
}

long long int gcd(long long int x, long long int y)
{
    x = iabs(x);
    y = iabs(y);
    long long int q;
    assert(y != 0);
    q = eu_mod(x, y);
    while (q != 0)
    {
        x = y;
        y = q;
        q = eu_mod(x, y);
    }
    return y;
}