#ifndef GREEKLATINGSQ_PERMUTATIONS_H
#define GREEKLATINGSQ_PERMUTATIONS_H

#include "../vector/Array2dInt.h"
#include "stdbool.h"

//<"=" == forbidden>
//<no move>
struct Permutations_struct
{

    ArrayInt perm_arr;

    Array2dInt available_slots;
    ArrayInt is_const_value;
    Array2dInt is_const_false_availability;

    int n;
    int amount_of_digits;


    bool did_start_filling;

    bool equality_lines_enabled;
    Array2dInt *equality_ways;

    Array2dInt are_available_from_equality_lines;
};


typedef struct Permutations_struct Permutations;

void initializePermutations(Permutations * res, int n, int dig_range, bool initialize_diagonal);
//diagonals are not initialized
void permutationsSetConstantValue(Permutations * res, int index, int value);

static inline int* atPermutations(Permutations * obj, int i)
{
    assert(i < obj->n);
    assert(i >= 0);
    return atArrayInt(&obj->perm_arr, i);
}

bool incrementPermutation(Permutations * perm);

void permutationsSetConstantValue(Permutations * res, int index, int value);

void permutationsSetConstFalseAvailability(Permutations * res, int index, int dig);

void printfPermutations(const Permutations * obj);

bool permutationsMakeMinFilling(Permutations * res);

void permutationsSetEqualityLines(Permutations * obj, Array2dInt *lines);
#endif //GREEKLATINGSQ_PERMUTATIONS_H
