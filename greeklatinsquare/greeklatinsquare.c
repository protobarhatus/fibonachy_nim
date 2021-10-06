#include "greeklatinsquare.h"
#include <stdbool.h>
#include "permutations.h"
#include <stdio.h>
#include <assert.h>
#include "user_interactions.h"
/*
 * basic idea here in recursive brute force with backtracking, but it's recursive relative to sections of specific
 * number. Different sections of each digit are gotten by incrementing Permutation, where on value of permutation[i]
 * is position (line) of digit on i column
 * on account of realization of Permutation, all invalid sections (including latin squares, but where
 * two same pairs of dig in source square, and searchable orthogonal occurs) are skipped
 * in 0 column, all digit go in order, because of symmetry
 * */

GreekLatinSquare defaultGreekLatinSquare(int n)
{
    GreekLatinSquare square;
    square.n = n;
    square.square = defaultArray2dInt(n, n, 0);
    return square;
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

void printfStage(ExecutionLogMode mode, double percent, double start_percent, int depth)
{
    switch (mode)
    {
        case EXECUTION_LOG_MODE_HEARTBEAT:
            printf(".");
        case EXECUTION_LOG_MODE_COUNTDOWN:
            printf("lvl %d: %f%% | %f%% searched\n", depth + 1, start_percent, percent * 100);
        default:
            return;
    }
}

double countPersent(int range, int n, Permutations * perm)
{
    double mult = 1. / range;
    double ac_mult = mult;
    double res = 0;
    for (int i = 0; i < n; ++i)
    {
        res += (*atPermutations(perm, i) - 1) * mult;
        mult *= ac_mult;
    }
    return res;
}


bool recursivePermutationGeneration(int n, int depth, GreekLatinSquare * res, int ans_required_number, int * counter, ExecutionLogMode mode,
                                    double start_percent)
{
    Permutations number_section;
    int i = 0, j = 0;

    if (depth == n)
        return true;

    initializePermutations(&number_section, n, n, false);
    setConstantNumbersInPermutation(n, res, &number_section);
    permutationsSetConstantValue(&number_section, 0, depth + 1);
    if (!permutationsMakeMinFilling(&number_section))
        return false;

    do
    {
        double percent = countPersent(n, n, &number_section) * n;
        if (n - depth > 7)
        {
            printfStage(mode, percent, start_percent, depth);
        }


        GreekLatinSquare attempt = *res;
        assignPermutationNumberPlacementToSquare(n, &attempt, &number_section, depth + 1);
        if (recursivePermutationGeneration(n, depth + 1, &attempt, ans_required_number, counter, mode,
                                           depth == 0 ? percent : start_percent))
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



GreekLatinSquare generateLatinSquare(int n, int ans_required_number, ExecutionLogMode mode)
{
    int counter = 0;
    GreekLatinSquare res = defaultGreekLatinSquare(n);

 /*   for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < n; ++j)
            *atGreekLatinSquare(&res, i, j) = ((i + j + 1) % n == 0 ? n : (i + j + 1) % n);
    }



    return res;*/

    recursivePermutationGeneration(n, 0, &res, ans_required_number, &counter, mode, 0);

    return res;
}



bool recursiveGenerateOrthogonal(int n, int depth, Array2dInt * source_indexed, GreekLatinSquare * res, ExecutionLogMode mode, double start_percent)
{
    Permutations number_section;
    int i = 0, j = 0;
   // double progress_mult = power(1. / fact(n), depth);

    if (depth == n)
        return true;

    initializePermutations(&number_section, n, n, false);
    setConstantNumbersInPermutation(n, res, &number_section);
    permutationsSetEqualityLines(&number_section, source_indexed);
    permutationsSetConstantValue(&number_section, 0, depth + 1);
    if (!permutationsMakeMinFilling(&number_section))
        return false;

    do
    {
        double percent = countPersent(n, n, &number_section) * n;
        if (n - depth > 7)
        {
            printfStage(mode, percent, start_percent, depth);
        }


        GreekLatinSquare attempt = *res;
        assignPermutationNumberPlacementToSquare(n, &attempt, &number_section, depth + 1);
        if (recursiveGenerateOrthogonal(n, depth + 1, source_indexed, &attempt, mode, (depth == 0 ? percent * 100 : start_percent)))
        {
            *res = attempt;
            return true;
        }
    } while (incrementPermutation(&number_section));

    return false;
}

GreekLatinSquare generateOrthogonal(GreekLatinSquare * source, ExecutionLogMode mode)
{
    if (source->n == 6)
        return defaultGreekLatinSquare(6);
    Array2dInt indexed_scource = makeIndexesWayArray(&source->square);
    GreekLatinSquare res = defaultGreekLatinSquare(source->n);
    recursiveGenerateOrthogonal(source->n, 0, &indexed_scource, &res, mode, 0);
    return res;
}

int * atGreekLatinSquare(GreekLatinSquare * obj, int i, int j);


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
            printf("%d%d ", *atGreekLatinSquare(a, i, j), *atGreekLatinSquare(b, i, j));
        }
        printf("\n");
    }
    printf("\n\n");
}

void fscanfGreekLatinSquare(FILE * file, GreekLatinSquare * obj, enum ScanfMode mode)
{
    int d, n;
    assert(file != NULL);

    n = getValue(file);
    *obj = defaultGreekLatinSquare(n);

    for (int i = 0; i < obj->n; ++i)
    {
        for (int j = 0; j < obj->n; ++j)
        {
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
