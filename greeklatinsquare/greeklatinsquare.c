#include "../exact_cover/exactcover.h"
#include "greeklatinsquare.h"
#include <stdbool.h>
#include "permutations.h"
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include "../user_interacts/user_interactions.h"

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



bool recursivePermutationGeneration(int n, int depth, GreekLatinSquare * res, int ans_required_number, int * counter)
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
        if (recursivePermutationGeneration(n, depth + 1, &attempt, ans_required_number, counter))
        {
            if (depth == n - 1 && *counter < ans_required_number)
            {
                (*counter) ++;
                return false;
            }
            *res = attempt;
            return true;
        }
    } while (incrementPermutation(&number_section));

    return false;
}



GreekLatinSquare generateLatinSquare(int n, int ans_required_number)
{
    int counter = 0;
    GreekLatinSquare res = defaultGreekLatinSquare(n);

 /*   for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < n; ++j)
            *atGreekLatinSquare(&res, i, j) = ((i + j + 1) % n == 0 ? n : (i + j + 1) % n);
    }



    return res;*/

    recursivePermutationGeneration(n, 0, &res, ans_required_number, &counter);

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
    //probably we can say that 1 should only be in left top corner, cause of symmetry ?
    if (depth == 0)
        permutationsSetConstantValue(&number_section, 0, 1);
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

GreekLatinSquare generateOrthogonalByBacktracking(GreekLatinSquare * source)
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

void printfLatinSquare(GreekLatinSquare * obj)
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

void printfGreekLatinSquare(GreekLatinSquare * a, GreekLatinSquare * b)
{
    int i = 0, j = 0;
    for (i = 0; i < a->n; ++i)
    {
        for (j = 0; j < b->n; ++j)
        {
            printf("%d%d", *atGreekLatinSquare(a, i, j), *atGreekLatinSquare(b, i, j));
        }
        printf("\n");
    }
    printf("\n\n");
}

void fscanfGreekLatinSquare(FILE * file, GreekLatinSquare * obj, enum ScanfMode mode)
{
    int d, n;
    assert(file != NULL);

   // fscanf(file, "%d", &n);
    n = getValue(file);
    *obj = defaultGreekLatinSquare(n);

    for (int i = 0; i < obj->n; ++i)
    {
        for (int j = 0; j < obj->n; ++j)
        {
           // fscanf(file,"%d", &d);
           d = getValue(file);
            if (mode == SCANF_MODE_DIRECT)
                *atGreekLatinSquare(obj, i, j) = (d + obj->n - 1) % obj->n + 1;
            else
                *atGreekLatinSquare(obj, i, j) = d / 10 + 1;
        }
    }
    if (!isLatin(obj))
    {
        printf("Error: input square is incorrect\n");
        getchar();
    }
}

bool inRange(int a, int x, int y)
{
    return a >= x && a <= y;
}
bool isLatin(GreekLatinSquare * square)
{
    for (int i = 0; i < square->n; ++i)
    {
        for (int j = 0; j < square->n; ++j)
            if (!inRange(*atGreekLatinSquare(square, i, j), 1, square->n))
                return false;
    }
    for (int i = 0; i < square->n; ++i)
    {
        ArrayInt is_taken = defaultArrayInt(square->n, false);
        for (int j = 0; j < square->n; ++j)
        {
            if (*atArrayInt(&is_taken, *atGreekLatinSquare(square, i, j) - 1))
                return false;
            *atArrayInt(&is_taken, *atGreekLatinSquare(square, i, j) - 1) = true;
        }
    }

    for (int j = 0; j < square->n; ++j)
    {
        ArrayInt is_taken = defaultArrayInt(square->n, false);
        for (int i = 0; i < square->n; ++i)
        {
            if (*atArrayInt(&is_taken, *atGreekLatinSquare(square, i, j) - 1))
                return false;
            *atArrayInt(&is_taken, *atGreekLatinSquare(square, i, j) - 1) = true;
        }
    }
    return true;
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

    if (!isLatin(a) || !isLatin(b))
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



#define r(T, L, X, Y) (T)->at(T, L, (X) * n + (Y))
#define c(T, L, X, Y) (T)->at(T, L, (X) * n + (Y) + n2)
#define x(T, L, X, Y) (T)->at(T, L, (X) * n + (Y) + 2*n2)
#define y(T, L, X, Y) (T)->at(T, L, (X) * n + (Y) + 3*n2)
void outOrthMMatrix(VectorVectorInt * matrix, int n)
{
    int n2 = n*n;
    int n3 = n*n*n;
    int out_l_in = 0;
    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            for (int k = 0; k < n; ++k)
            {
                if (j == 0 && i != k)
                    continue;
                for (int t = 0; t < n; ++t)
                {
                    for (int w = 0; w < n; ++w)
                    {
                        printf("%d %d %d %d ", *r(matrix, out_l_in, t, w),
                               *c(matrix, out_l_in, t, w), *x(matrix, out_l_in, t, w), *y(matrix, out_l_in, t, w));

                    }
                }
                printf("\n");
                ++out_l_in;
            }
        }
    }
    printf("-----------------------------------\n\n");
}

VectorVectorInt squareToExactCover(GreekLatinSquare * source)
{
    int n = source->n;
    int n2 = n*n;
    int n3 = n*n*n;

    VectorVectorInt matrix = defaultVectorVectorIntCalloc(n3 - n2 + n, defaultVectorIntCalloc(4 * n2, 0));

    int line_index = 0;
    //outOrthMMatrix(&matrix, n);
    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            for (int k = 0; k < n; ++k)
            {
                if (j == 0 && !(i == k))
                    continue;
             //   if (j == 0 && i == k)
              //      force_included_values.pushBack(&force_included_values, line_index);
                *r(&matrix, line_index, i, k) = 1;
                // outOrthMMatrix(&matrix, n);
                *c(&matrix, line_index, j, k) = 1;
                //  outOrthMMatrix(&matrix, n);
                *x(&matrix, line_index, i, j) = 1;
                // outOrthMMatrix(&matrix, n);
                *y(&matrix, line_index, *atGreekLatinSquare(source, i, j) - 1, k) = 1;
                ++line_index;
                //  outOrthMMatrix(&matrix, n);
            }
        }
    }
    return matrix;
}
static int root(int n)
{
    double v = sqrt(n);
    int v1 = v;
    int v2 = v + 1;
    int v3 = v - 1;
    if (v1 * v1 == n)
        return v1;
    if (v2 * v2 == n)
        return v2;
    if (v3 * v3 == n)
        return v3;
    return -1;
}
GreekLatinSquare exactCoverToSquare(VectorInt * exact_cover)
{
    int n = root(exact_cover->getSize(exact_cover));
    int n2 = n*n;
    if (n == -1)
        return defaultGreekLatinSquare(0);
    GreekLatinSquare res = defaultGreekLatinSquare(n);

    for (int i = 0; i < n; ++i)
        *atGreekLatinSquare(&res, i, 0) = i + 1;

    int counter = 0;
    int shift = 0;
    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            if (j == 0){
                shift += n - 1;
                ++counter;
                continue;
            }
            *atGreekLatinSquare(&res, i, j) = (*exact_cover->at(exact_cover, counter) + shift) % n + 1;
            ++counter;
        }
    }
    return res;
}

GreekLatinSquare generateOrthogonalByExactCover(GreekLatinSquare * source)
{


    VectorVectorInt matrix = squareToExactCover(source);

    VectorInt exact_cover = makeExactCover(&matrix);
  //  printfVectorInt("", &exact_cover);

    int n = source->n;
    int n2 = n*n;
    int n3 = n*n*n;

    if(exact_cover.getSize(&exact_cover) != n2)
    {
        destructVectorInt(&exact_cover);
        destructVectorVectorInt(&matrix);
        return defaultGreekLatinSquare(n);
    }

    GreekLatinSquare res = exactCoverToSquare(&exact_cover);

    destructVectorInt(&exact_cover);
    destructVectorVectorInt(&matrix);

    return res;

#undef r
#undef c
#undef x
#undef y

}

bool isDefaultSquare(GreekLatinSquare * square)
{
    for (int i = 0; i < square->n; ++i)
    {
        for (int j = 0; j < square->n; ++j)
            if (*atGreekLatinSquare(square, i, j) != 0)
                return false;
    }
    return true;
}