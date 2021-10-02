#include "greeklatinsquare.h"
#include <stdbool.h>
#include "permutations.h"
#include <stdio.h>

GreekLatinSquare defaultGreekLatinSquare(int n)
{
    GreekLatinSquare square;
    square.n = n;
    square.square = defaultArray2dInt(n, n, 0);
    return square;
}

void setConstantNumbersInPermutation(int n, GreekLatinSquare * obj, Permutations * res)
{
    int i = 0, j = 0;
    for (i = 0; i < n; ++i)
    {
        for (j = 0; j < n; ++j)
        {
            if (*atGreekLatinSquare(obj, j, i) != 0)
                permutationsSetConstFalseAvailability(res, i, j + 1);
        }
    }
}

void assignPermutationNumberPlacementToSquare(int n, GreekLatinSquare * res, Permutations * perm, int cur_digit)
{
    int i = 0;
    for (i = 0; i < n; ++i)
    {
        *atGreekLatinSquare(res, *atPermutations(perm, i) - 1, i) = cur_digit;
    }
}



bool recursivePermutationGeneration(int n, int depth, GreekLatinSquare * res)
{
    Permutations number_section;
    int i = 0, j = 0;

    if (depth == n)
        return true;

    initializePermutations(&number_section, n, n, false);
    setConstantNumbersInPermutation(n, res, &number_section);
    if (!permutationsMakeMinFilling(&number_section))
        return false;

    do
    {
        GreekLatinSquare attempt = *res;
        assignPermutationNumberPlacementToSquare(n, &attempt, &number_section, depth + 1);
        if (recursivePermutationGeneration(n, depth + 1, &attempt))
        {
            *res = attempt;
            return true;
        }
    } while (incrementPermutation(&number_section));

    return false;
}



GreekLatinSquare generateGreekLatinSquare(int n)
{
    GreekLatinSquare res = defaultGreekLatinSquare(n);

 /*   for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < n; ++j)
            *atGreekLatinSquare(&res, i, j) = ((i + j + 1) % n == 0 ? n : (i + j + 1) % n);
    }



    return res;*/

    recursivePermutationGeneration(n, 0, &res);

    return res;
}



bool recursiveGenerateOrthogonal(int n, int depth, Array2dInt * source_indexed, GreekLatinSquare * res)
{
    Permutations number_section;
    int i = 0, j = 0;

    if (depth == n)
        return true;

    initializePermutations(&number_section, n, n, false);
    setConstantNumbersInPermutation(n, res, &number_section);
    permutationsSetEqualityLines(&number_section, source_indexed);
    if (!permutationsMakeMinFilling(&number_section))
        return false;

    do
    {
        GreekLatinSquare attempt = *res;
        assignPermutationNumberPlacementToSquare(n, &attempt, &number_section, depth + 1);
        if (recursiveGenerateOrthogonal(n, depth + 1, source_indexed, &attempt))
        {
            *res = attempt;
            return true;
        }
    } while (incrementPermutation(&number_section));

    return false;
}

GreekLatinSquare generateOrthogonal(GreekLatinSquare * source)
{
    Array2dInt indexed_scource = makeIndexesWayArray(&source->square);
    GreekLatinSquare res = defaultGreekLatinSquare(source->n);
    recursiveGenerateOrthogonal(source->n, 0, &indexed_scource, &res);
    return res;
}

int * atGreekLatinSquare(GreekLatinSquare * obj, int i, int j)
{
    return atArray2dInt(&obj->square, i, j);
}

void printfGreekLatinSquare(GreekLatinSquare * obj)
{
    int i = 0, j = 0;
    for (i = 0; i < obj->n; ++i)
    {
        for (j = 0; j < obj->n; ++j)
        {
            printf("%d ", *atGreekLatinSquare(obj, i, j));
        }
        printf("\n");
    }
    printf("\n\n");
}

void scanfGreekLatinSquare(GreekLatinSquare * obj, enum ScanfMode mode)
{
    int d;
    for (int i = 0; i < obj->n; ++i)
    {
        for (int j = 0; j < obj->n; ++j)
        {
            scanf("%d", &d);
            if (mode == SCANF_MODE_DIRECT)
                *atGreekLatinSquare(obj, i, j) = (d + obj->n) % obj->n + 1;
            else
                *atGreekLatinSquare(obj, i, j) = d / 10 + 1;
        }
    }
}

bool hasPairRepetition(GreekLatinSquare * a, GreekLatinSquare * b, int i_prime, int j_prime)
{
    for (int i = 0; i < a->n; ++i)
    {
        for (int j = 0; j < a->n; ++j)
        {
            if (!(i == i_prime && j == j_prime) && *atGreekLatinSquare(a, i, j) == *atGreekLatinSquare(a, i_prime, j_prime) &&
                    *atGreekLatinSquare(b, i, j) == *atGreekLatinSquare(b, i_prime, j_prime))
                return true;
        }
    }
    return false;
}

bool areOrthogonal(GreekLatinSquare * a, GreekLatinSquare * b)
{
    if (a->n != b->n)
        return false;

    for (int i = 0; i < a->n; ++i)
    {
        for (int j = 0; j < b->n; ++j)
        {
            if (hasPairRepetition(a, b, i, j))
                return false;
        }
    }
    return true;
}