/*
* so as i couldn't come up with more efficient, mathematical idea (if its exist), i do brute force all variants
* the system is defined by inner polygon (circle in terms of programm), and triplet's sum
* so program at first makes all variations of inner polygon and then tries to build to them different outter polygon by brute forcing third member of one of triplet, that
* is equivalent to triplet's sum. if it can fill out circle, than it one of answers
*
* all variations of inner polygon is all variations of filling n places with 2*n different numbers and are made by this way:
* min element is fixed in [0] position, so it prevents cyclical repeats. min element may be from 1 to n + 1. for convience, variations for each min elements are done separatly
* Assuming, min element m is fixed, first variation is minimal possible in lexicographical order: m, m + 1, m + 2, ... , m + n - 1
* then all variations are made by step-by-step incrementing previous variant. increment is made like of regular number in 2*n base with condition that digit may take only one place
* in number and no digits less than m are allowed
*
* on each step, variation is accompanied by table that remembers what digit can or cannot be used on specific place
* its needed to not look back for every digit finding if it already present in number
* that table increments simultaniously with variation
*
* variations would have clock-wise and counterclock-wise symmetry pair, but i keeping it because result with them are actually different
* so each variation with its symmetry pair can provide (if its provide anything at all) at least 4 different variants
*
* the filling of polygon is saved in struct CircleFilling, that keeps not only array with filling, but also boolean table that tells witch numbers of 1..2*n are present in that filling
* it's very helpful to finding out filling of outter polygon cause for every number we should'nt scan inner polygon to find out if it is taken
*/


#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>


long long int binomialCoefficient(int n, int k)
{
    const int N = 30;
    static long long int binomial_coefficients[N + 1][N + 1];
    int i = 0, j = 0;
    static bool was_builded = false;

    assert(n <= 30);
    assert(n >= k);
    if (was_builded)
    {
        return binomial_coefficients[n][k];
    }
    was_builded = true;
    
    binomial_coefficients[0][0] = 1;
    binomial_coefficients[1][0] = 1;
    binomial_coefficients[1][1] = 1;
    for (i = 2; i <= N; ++i)
    {
        binomial_coefficients[i][0] = 1;
        binomial_coefficients[i][i] = 1;
        for (j = 1; j < i; ++j)
            binomial_coefficients[i][j] = binomial_coefficients[i - 1][j - 1] + binomial_coefficients[i - 1][j];
    }

    return binomial_coefficients[n][k];
}

int ceilDiv(int n, int d)
{
    if (n % d == 0)
        return n / d;
    return n / d + 1;
}

#define MAX_COMB_ARRAY_LEN 31
int* atDoubleArrayInt(int x, int y, int first_size, int * arr)
{
    return &arr[x * first_size + y];
}
bool* atDoubleArrayBool(int x, int y, int first_size, bool* arr)
{
    return &arr[x * first_size + y];
}

int findDigitsIncrement(int i, int d_val, int n, bool* awaylable_slots)
{
    for (d_val = d_val + 1; d_val <= 2*n; ++d_val)
        if (*atDoubleArrayBool(i, d_val, MAX_COMB_ARRAY_LEN, awaylable_slots))
            return d_val;
    return -1;

}

int findMinDigit(int index, int n, bool* awaylable_slots)
{
    int i = 0;
    for (i = 1; i <= 2*n; ++i)
    {
        if (*atDoubleArrayBool(index, i, MAX_COMB_ARRAY_LEN, awaylable_slots))
            return i;
    }
    return -1;
}

void setAwaylabilityToAllDigitsOfAndAfter(int pos, int dig, int n, bool value, bool* awaylable_slots)
{
    int i = 0;
    for (i = pos; i < n; ++i)
    {
        *atDoubleArrayBool(i, dig, MAX_COMB_ARRAY_LEN, awaylable_slots) = value;
    }
}

int fact(int n)
{
    int i = 0, res = 1;
    assert(n <= 12);
    for (i = 1; i <= n; ++i)
        res *= i;
    return res;
}
bool incrementCombs(const int* previous, int* dest, bool* awaylable_slots, int n)
{
    int incr_lab = n - 1;
    int i = 0, j = 0;
    //as 0 position is taken by 1 always

    for (int i = 1; i < n; ++i)
        dest[i] = previous[i];
    for (incr_lab = n - 1; incr_lab > 0; --incr_lab)
    {
        int dig_incr = findDigitsIncrement(incr_lab, previous[incr_lab], 2*n, awaylable_slots);
     //   if (incr_lab == (n - 1) / 2 && previous[incr_lab] == 2)
      //      continue;
        setAwaylabilityToAllDigitsOfAndAfter(incr_lab, previous[incr_lab], n, true, awaylable_slots);
        if (dig_incr > 0)
        {
            setAwaylabilityToAllDigitsOfAndAfter(incr_lab, previous[incr_lab], n, true, awaylable_slots);
            dest[incr_lab] = dig_incr;
            setAwaylabilityToAllDigitsOfAndAfter(incr_lab, dig_incr, n, false, awaylable_slots);
            break;
        }
    }

    if (incr_lab <= 0)
        return false;

    for (incr_lab += 1; incr_lab < n; ++incr_lab)
    {
        int its_dig = findMinDigit(incr_lab, 2*n, awaylable_slots);
        assert(its_dig > 0);
        //setAwaylabilityToAllDigitsOfAndAfter(incr_lab, previous[incr_lab], n, true, awaylable_slots);
        dest[incr_lab] = its_dig;
        setAwaylabilityToAllDigitsOfAndAfter(incr_lab, its_dig, n, false, awaylable_slots);
    }

    return true;
}

int min(int a, int b)
{
    return a < b ? a : b;
}

bool isMinOnSecondHalf(int* combstr, int n)
{
    int i = 0;
    int min_d = combstr[1];
    int premin = 0;
    bool is_min_on_symmetry_axis = false;
    for (i = 2; i <= n / 2; ++i)
    {
        if (combstr[i] < min_d && i == (n / 2))
            is_min_on_symmetry_axis = true;
        min_d = min(combstr[i], min_d);
    }
    for (i = n / 2 + 1; i < n; ++i)
    {
        //<= is so that it easily can throw away empty arrays like a 0 0 0 ...
        if (combstr[i] <= min_d)
            return true;
    }

    if (is_min_on_symmetry_axis && (n - 1) % 2 == 1)
    {
        premin = 2 * n;
        for (int i = 1; i < n / 2; ++i)
        {
            premin = min(premin, combstr[i]);
        }
        for (int i = n / 2 + 1; i < n; ++i)
        {
            if (combstr[i] <= premin)
                return true;
        }
    }
    return false;
}

void outAwaylablesTable(bool* table, int n)
{
    int i = 0, j = 0;
    printf("   |");
    for (i = 0; i < n; ++i)
    {
        printf("%d ", i + 1);
    }
    printf("\n");
    for (i = 1; i <= 2*n; ++i)
    {
        printf("%d  |", i);
        for (j = 0; j < n; ++j)
            printf("%d ", *atDoubleArrayBool(j, i, MAX_COMB_ARRAY_LEN, table));
        printf("\n");
    }
    printf("-----------\n");
}

void outArray(int* arr, int n)
{
    int i;
    for (i = 0; i < n; ++i)
        printf("%d ", arr[i]);
    printf("\n");
}

struct CircleFilling
{
    int* inner_fill = NULL;
    bool has_number[2 * MAX_COMB_ARRAY_LEN] = { false };
};

void createCircleFilling(CircleFilling* obj, int n)
{
    obj->inner_fill = (int*)calloc(n, sizeof(int));

    assert(obj->inner_fill != NULL);

    for (int i = 0; i < 2 * MAX_COMB_ARRAY_LEN; ++i)
        obj->has_number[i] = false;
}

CircleFilling* callocCircleFilling(int n)
{
    CircleFilling* obj = (CircleFilling*)calloc(1, sizeof(CircleFilling));

    assert(obj != NULL);

    createCircleFilling(obj, n);
    return obj;
}

void destructCircleFilling(CircleFilling* obj)
{
    if (obj->inner_fill != NULL)
        free(obj->inner_fill);
    obj->inner_fill = NULL;
}

void moveCircleFilling(CircleFilling* what, CircleFilling* dest)
{
    destructCircleFilling(dest);
    int i = 0;
    dest->inner_fill = what->inner_fill;
    what->inner_fill = NULL;
    memcpy((void*)dest, (void*)what, sizeof(bool) * 2 * MAX_COMB_ARRAY_LEN);
}

void circleFillingAddNumber(CircleFilling* circle, int num, int index)
{
    circle->inner_fill[index] = num;
    circle->has_number[num] = true;
}

bool circleFillingHasNumber(CircleFilling* obj, int num)
{
    return obj->has_number[num];
}

int circleFillingGetInnerFillNumber(CircleFilling* obj, int index)
{
    return obj->inner_fill[index];
}



CircleFilling* buildInnerFillVariations(int n)
{
    assert(n <= 12);
   // printf("%d\n", binomialCoefficient(2 * n - 1, n - 1));
    int one_block_size = binomialCoefficient(2 * n - 1, n - 1) * fact(n - 1);
    long long int size = (n + 1) * one_block_size;

    int** inner_fill = (int**)calloc(size + 1, sizeof(int*));
    assert(inner_fill != NULL);

    CircleFilling* answ_inner_fill = (CircleFilling*)calloc(size + 1, sizeof(CircleFilling));
    assert(answ_inner_fill != NULL);

    int i = 0, j = 0, k = 0;
    bool incremented_awailable_slots[MAX_COMB_ARRAY_LEN * 2 * MAX_COMB_ARRAY_LEN] = { false };
    int true_coms_arr_size = 1;

    assert(n >= 3);

    inner_fill[0] = (int*)calloc(2, sizeof(int));
    assert(inner_fill != NULL);

    for (i = 1; i <= size; ++i)
    {
        inner_fill[i] = (int*)calloc(n, sizeof(int));
        assert(inner_fill[i] != NULL);
        inner_fill[i][0] = 1 + ((i - 1) / one_block_size);
    }

    for (i = 0; i < n + 1; ++i)
    {
        for (j = 0; j < n; ++j)
        {
            for (k = 0; k <= 2*n; ++k)
                *atDoubleArrayBool(j, k, MAX_COMB_ARRAY_LEN, incremented_awailable_slots) = false;
        }

        for (j = 1; j < n; ++j)
        {
            inner_fill[1 + i * one_block_size][j] = (j + 1) + i;
            for (k = j + 2 + i; k <= 2 * n; ++k)
                *atDoubleArrayBool(j, k, MAX_COMB_ARRAY_LEN, incremented_awailable_slots) = true;
        }

        for (int j = 1; j < one_block_size; ++j)
        {
           // if (inner_fill[1 + i * one_block_size + j - 1][0] > 1)
           //     scanf("%d", &i);
          //  printf("<%d> <%d>\n", incremented_awailable_slots[1][3], *atDoubleArrayBool(1, 3, MAX_COMB_ARRAY_LEN, incremented_awailable_slots));
        //    outAwaylablesTable(incremented_awailable_slots, n);
          //  outArray(inner_fill[1 + i * one_block_size + j - 1], n);
            
            if (!incrementCombs(inner_fill[1 + i * one_block_size + j - 1],
                                    inner_fill[1 + i * one_block_size + j], incremented_awailable_slots, n))
                break;
        }

    }

    answ_inner_fill[0].inner_fill = (int *)calloc(2, sizeof(int));
    assert(answ_inner_fill[0].inner_fill != NULL);

    for (i = 1; i < size + 1; ++i)
    {
       // if (!isMinOnSecondHalf(inner_fill[i], n))
        {
            answ_inner_fill[true_coms_arr_size].inner_fill = inner_fill[i];
            for (j = 0; j < n; ++j)
                answ_inner_fill[true_coms_arr_size].has_number[inner_fill[i][j]] = true;
            inner_fill[i] = NULL;
            ++true_coms_arr_size;
        }

    }
    answ_inner_fill[0].inner_fill[0] = true_coms_arr_size;
    answ_inner_fill[0].inner_fill[1] = size + 1;

    free(inner_fill);
    return answ_inner_fill;

}

CircleFilling* tryPlacementWithFixedSum(CircleFilling* inner_placement, int n, int sum, int dir)
{
    CircleFilling* out_circle = callocCircleFilling(n);
    
    for (int i = 0; i < n; ++i)
    {
        int addict = sum - circleFillingGetInnerFillNumber(inner_placement, i) - circleFillingGetInnerFillNumber(inner_placement, (i + dir + n) % n);
        if (addict <= 0 || addict > 2 * n || circleFillingHasNumber(inner_placement, addict) || circleFillingHasNumber(out_circle, addict))
        {
            destructCircleFilling(out_circle);
            free(out_circle);
            return NULL;
        }
        circleFillingAddNumber(out_circle, addict, i);
    }
    return out_circle;
}
CircleFilling* findOutPlacement(CircleFilling* inner_placement, int n, int dir)
{
    int i = 0;
    for (i = 1; i <= 2 * n; ++i)
    {
        if (!circleFillingHasNumber(inner_placement, i))
        {
            CircleFilling* res = tryPlacementWithFixedSum(inner_placement, n, i + circleFillingGetInnerFillNumber(inner_placement, 0) +
                                                                                         circleFillingGetInnerFillNumber(inner_placement, (dir + n) % n), dir);
            if (res != NULL)
                return res;

        }
    }
    return NULL;
}

CircleFilling** findAllOutFillings(CircleFilling* inner_variations, int n, int amount_of_variations)
{
    int i = 0;
    CircleFilling **out_fillings = (CircleFilling**)calloc(2 * amount_of_variations, sizeof(CircleFilling*));
    assert(out_fillings != NULL);
    for (i = 1; i < amount_of_variations; ++i)
    {
        //outArray(inner_variations[i].inner_fill, n);
        CircleFilling* filling_res = findOutPlacement(&inner_variations[i], n, 1);
        if (filling_res != NULL)
        {
            out_fillings[i] = filling_res;
            filling_res = NULL;
        }

    }
    for (i = amount_of_variations; i < 2*amount_of_variations; ++i)
    {
        //outArray(inner_variations[i].inner_fill, n);
        CircleFilling* filling_res = findOutPlacement(&inner_variations[i - amount_of_variations], n, -1);
        if (filling_res != NULL)
        {
            out_fillings[i] = filling_res;
            filling_res = NULL;
        }

    }
    return out_fillings;
}

int findMinIndex(CircleFilling* circle, int n)
{
    int min_d = 2 * n;
    int i = 0;
    int res_index = 0;
    for (i = 0; i < n; ++i)
    {
        if (circleFillingGetInnerFillNumber(circle, i) < min_d)
        {
            min_d = circleFillingGetInnerFillNumber(circle, i);
            res_index = i;
        }
    }
    return res_index;
}

void printfTriplet(int a, int b, int c)
{
    printf("%d, %d, %d;   ", a, b, c);
}
void outCircle(CircleFilling* inner_circle, CircleFilling* outer_circle, int n)
{
    int start = findMinIndex(outer_circle, n);
    int i = 0;
    for (i = 0; i < n; ++i)
    {
        printfTriplet(circleFillingGetInnerFillNumber(outer_circle, (i + start) % n),
                        circleFillingGetInnerFillNumber(inner_circle, (i + start) % n),
                            circleFillingGetInnerFillNumber(inner_circle, (i + start + 1) % n));
    }
    printf("\n");
}

int main()
{
    int n = 0;

    if (!scanf("%d", &n))
        abort();

    CircleFilling* variations = buildInnerFillVariations(n);
    CircleFilling** out_circles = NULL;
    int i = 0;
  /*  for (int i = 1; i < variations[0].inner_fill[0]; ++i)
    {
        for (int j = 0; j < 4; ++j)
            printf("%d ", variations[i].inner_fill[j]);
        printf("\n");
    }
	return 0;*/

    out_circles = findAllOutFillings(variations, n, circleFillingGetInnerFillNumber(variations, 0));

    for (i = 1; i < 2*circleFillingGetInnerFillNumber(&variations[0], 0); ++i)
    {
        if (out_circles[i] != NULL)
        {
            outCircle(&variations[i % circleFillingGetInnerFillNumber(&variations[0], 0)], out_circles[i], n);
        }
    }
}