#include "permutations.h"
#include <assert.h>
#include <stdio.h>

/*
 * class represent different permutations of some set in form of digit with dig_range based system, n digits
 * and condition on its digits of no repetitions
 * thus, different digits represent all variants of permutations and only them, their amount equal A[dig_range][n]
 * in Increment function, permutation is replaced by next, assuming operation less is standard
 * in incremention, function check if it can increment last digit. If yes, then it's done.
 * If no, digit is removed and pointer goes down. eventually, it will find digit (if no, then permutation if max)
 * that digit is replaced by next available and then it goes backwards, setting minimum available digits
 * to speed up incrementation, class contains boolean array that tells if digit is available
 * and is changed every time digit gets placed/removed
 *
 * but to skip variants that are forbidden in orthogonal square (pair repetition),
 * class implements equality_lines (inside class they are considered in abstract from latin squares). in this array,
 * n = equality_lines[i][j], where i is index of place, j is digit, and n is digit in i+1 column that is
 * forbidden if in i slot there is j digit (that represent that if in i column of latin square is digit k,
 * setting digit in orthogonal to k forbids all lines in next columns where k is present. n is line of k in next column,
 * information about k itself is not saved)
 *
 * that also complicates incrementation, because now, after incremented digit is found its necessary to make try-and-repeat
 * method to set new digits, but amount of skipped permutations is worth it
 */
typedef struct Permutations_struct Permutations;

void setAvailabilityToAllDigitsOfAndAfter(Permutations * obj, int pos, int dig, int n, bool value);

void initializePermutations(Permutations * res, int n, int dig_range, bool initialize_diagonal)
{
    int i = 0;
    res->perm_arr = defaultArrayInt(n, 0);
    res->available_slots = defaultArray2dInt(n, dig_range + 1, true);
    res->is_const_value = defaultArrayInt(n, false);
    res->is_const_false_availability = defaultArray2dInt(n, dig_range + 1, false);
    res->n = n;
    res->amount_of_digits = dig_range;

    res->did_start_filling = initialize_diagonal;

    res->equality_lines_enabled = false;

    setAvailabilityToAllDigitsOfAndAfter(res, 0, 0, n, false);
    if (initialize_diagonal)
    {
        for (i = 0; i < n; ++i) {
            *atArrayInt(&res->perm_arr, i) = i + 1;
            setAvailabilityToAllDigitsOfAndAfter(res, i, i + 1, n, false);
        }
    }

}
//diagonals are not initialized
void permutationsSetConstantValue(Permutations * res, int index, int value)
{
    *atArrayInt(&res->perm_arr, index) = value;
    setAvailabilityToAllDigitsOfAndAfter(res, 0, value, res->n, false);
    *atArrayInt(&res->is_const_value, index) = true;
}

void permutationsSetConstFalseAvailability(Permutations * res, int index, int dig)
{
    *atArray2dInt(&res->is_const_false_availability, index, dig) = true;
    *atArray2dInt(&res->available_slots, index, dig) = false;
}

int* atPermutations(Permutations * obj, int i);

bool isSlotAvailable(Permutations * obj, int index, int dig)
{
    if (*atArrayInt(&obj->is_const_value, index))
        return false;
    if (!(*atArray2dInt(&obj->available_slots, index, dig)))
        return false;
    if (obj->equality_lines_enabled)
    {
        return *atArray2dInt(&obj->are_available_from_equality_lines, index, dig);
    }

    return true;
}

int findDigitsIncrement(Permutations * obj, int i, int d_val, int dig_range)
{
    for (d_val = d_val + 1; d_val <= dig_range; ++d_val)
        if (isSlotAvailable(obj, i, d_val))
            return d_val;
    return -1;

}

int findMinDigit(Permutations  * obj, int index, int dig_range)
{
    int i = 0;
    for (i = 1; i <= dig_range; ++i)
    {
        if (isSlotAvailable(obj, index, i))
            return i;
    }
    return -1;
}

void setAvailabilityToAllDigitsOfAndAfter(Permutations * obj, int pos, int dig, int n, bool value)
{
    int i = 0;
    for (i = pos; i < n; ++i)
    {
        if (!(*atArray2dInt(&obj->is_const_false_availability, i, dig)))
            *atArray2dInt(&obj->available_slots, i, dig) = value;
    }
    if (obj->equality_lines_enabled)
    {

      int next_eq_line_index = dig;
        for (i = pos; i < n; ++i)
        {

            if (!(*atArray2dInt(&obj->is_const_false_availability, (0 + i + 0) % n, next_eq_line_index)))
                *atArray2dInt(&obj->are_available_from_equality_lines, (0 + i + 0) % n, next_eq_line_index) = value;
            next_eq_line_index = *atArray2dInt(obj->equality_ways, i, next_eq_line_index - 1) + 1;
        }
    }
}

bool tryIncrementOnThatPlace(Permutations * res, ArrayInt * dest, int n, int dig_range,
                             int incr_lab, int dig_incr)
{
    ArrayInt tried_digits = defaultArrayInt(n - incr_lab, 0);
    int start_incr_lab = incr_lab;
    if (!(*atArrayInt(&res->is_const_value, incr_lab)))
    {
        if (dig_incr == 0)
            dig_incr = findMinDigit(res, 0, dig_range);
        if (dig_incr == -1)
            return false;
        *atArrayInt(&tried_digits, 0) = dig_incr;

        *atArrayInt(dest, incr_lab) = dig_incr;
        setAvailabilityToAllDigitsOfAndAfter(res, incr_lab, dig_incr, n, false);
    }

    for (incr_lab += 1; incr_lab < n && incr_lab >= start_incr_lab; ++incr_lab)
    {
        //printfArrayInt(&tried_digits);
      //  if (*atArrayInt(&res->is_const_value, incr_lab))
      //      continue;

        if (*atArrayInt(&tried_digits, incr_lab - start_incr_lab) > 0)
            setAvailabilityToAllDigitsOfAndAfter(res, incr_lab, *atArrayInt(&tried_digits, incr_lab - start_incr_lab), n, true);

        int its_dig = findDigitsIncrement(res, incr_lab, *atArrayInt(&tried_digits, incr_lab - start_incr_lab), dig_range);

        if (its_dig < 0)
        {
      //      if (*atArrayInt(&tried_digits, incr_lab - start_incr_lab) > 0)
      //          setAvailabilityToAllDigitsOfAndAfter(res, incr_lab, *atArrayInt(&tried_digits, incr_lab - start_incr_lab), n, true);
            if (incr_lab == start_incr_lab)
                return false;

            if (*atArrayInt(&tried_digits, incr_lab - start_incr_lab) > 0)
                setAvailabilityToAllDigitsOfAndAfter(res, incr_lab, *atArrayInt(&tried_digits, incr_lab - start_incr_lab), n, true);
            *atArrayInt(&tried_digits, incr_lab - start_incr_lab) = 0;
            incr_lab -= 2;
            continue;
        }
        assert(its_dig > 0);

        *atArrayInt(&tried_digits, incr_lab - start_incr_lab) = its_dig;
        //setAwaylabilityToAllDigitsOfAndAfter(incr_lab, previous[incr_lab], n, true, awaylable_slots);
        *atArrayInt(dest, incr_lab) = its_dig;
        setAvailabilityToAllDigitsOfAndAfter(res, incr_lab, its_dig, n, false);
        *atArrayInt(&tried_digits, incr_lab - start_incr_lab) = its_dig;
    }
    return true;
}

bool makeIncrementation(Permutations * res, ArrayInt * dest, int n, int dig_range)
{
    int incr_lab = n - 1;
    int i = 0, j = 0;

    for (incr_lab = n - 1; incr_lab >= 0; --incr_lab)
    {
        if (*atArrayInt(&res->is_const_value, incr_lab))
            continue;

        int dig_incr = findDigitsIncrement(res, incr_lab, *atArrayInt(dest, incr_lab), dig_range);
        //   if (incr_lab == (n - 1) / 2 && previous[incr_lab] == 2)
        //      continue;
        setAvailabilityToAllDigitsOfAndAfter(res, incr_lab, *atArrayInt(dest, incr_lab), n, true);
        if (dig_incr > 0)
        {
          //  setAvailabilityToAllDigitsOfAndAfter(res, incr_lab, *atArrayInt(previous, incr_lab), n, true);
            if (tryIncrementOnThatPlace(res, dest, n, dig_range, incr_lab, dig_incr))
                return true;
        }
    }

    return false;


}

static void swap(ArrayInt ** a, ArrayInt ** b)
{
    ArrayInt  * t = *a;
    *a = *b;
    *b = t;
}

bool incrementPermutation(Permutations * perm)
{
    assert(perm->did_start_filling);
    return makeIncrementation(perm, &perm->perm_arr, perm->n, perm->amount_of_digits);
}

bool permutationsMakeMinFilling(Permutations * res)
{
    assert(!res->did_start_filling);
    res->did_start_filling = true;
   // int dig = findMinDigit(res, 0, res->amount_of_digits);

   // if (dig == -1)
   //     return false;
    return tryIncrementOnThatPlace(res, &res->perm_arr, res->n, res->amount_of_digits, 0, 0);
   /* for (int i = 0; i < res->n; ++i)
    {
        if (*atArrayInt(res->main_arr, i) != 0)
            continue;

        int dig = findMinDigit(res, i, res->amount_of_digits);

        if (dig == -1)
            return false;

       // *atArrayInt(res->main_arr, i) = dig;
       // setAvailabilityToAllDigitsOfAndAfter(res, i, dig, res->n, false);

     //  if (tryIncrementOnThatPlace(res, res->main_arr, res->main_arr, res->n, res->amount_of_digits, i, dig))
       //    return true;
    }
    return true;*/
}


void printfPermutations(const Permutations * obj)
{
    int i = 0;
    for (i = 0; i < obj->n; ++i)
    {
        printf("%d ", *atPermutations(obj, i));
    }
    printf("\n");
}

void permutationsSetEqualityLines(Permutations * obj, Array2dInt * lines)
{
    assert(!obj->did_start_filling);
    obj->equality_lines_enabled = true;
    obj->equality_ways = lines;
    obj->are_available_from_equality_lines = defaultArray2dInt(obj->n, obj->amount_of_digits + 1, true);
}

