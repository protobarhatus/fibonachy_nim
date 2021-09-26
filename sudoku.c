#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <stdbool.h>
#include <assert.h>

struct Array2d
{
    int w, h;
    int arr[81];
};

struct  Array2d defaultArray2d(int w, int h)
{
    struct Array2d arr;
    arr.w = w;
    arr.h = h;
    memset((void *)arr.arr, 0, sizeof(int) * 81);
    return arr;
}
int * atArray2d(struct  Array2d * obj, int x, int y)
{
    return &obj->arr[x * obj->w + y];
}
#define BOOL_ARR_SIZE 10
struct Array2dBoolArray
{
    int w, h;
    bool arr[BOOL_ARR_SIZE * 81];
};

struct Array2dBoolArray defaultArray2dBoolArray(int w, int h, bool def_value)
{
    struct Array2dBoolArray arr;
    arr.w = w;
    arr.h = h;
    memset((void *)arr.arr, def_value, sizeof(int) * 81);
    return arr;
}
bool * atArray2dBoolArray(struct Array2dBoolArray * obj, int x, int y)
{
    return &obj->arr[(x * obj->w + y) * BOOL_ARR_SIZE];
}


struct PointerTuple
{
    void * pointers;
    int size;
};

struct PointerTuple initializePointerTuple(int size, ...)
{
    int i = 0;
    void * act_pointer = (void *)((&size) + 1);
    struct PointerTuple tuple;
    tuple.size = size;
    for (i = 0; i < size; ++i)
    {
        *((&tuple.pointers + i * sizeof(int*))) = act_pointer;
        act_pointer += sizeof(int *);
    }
    return tuple;
}
void * atPointerTuple(struct PointerTuple * obj, int i)
{
    assert(i < obj->size);
    return *(&obj->pointers + sizeof(int *) * i);
}


struct Sudoku
{
    struct Array2d sudoku;
    struct Array2dBoolArray possible_states;
};

*atSudoku(struct Sudoku * obj, int x, int y)
{
    return atArray2d(&obj->sudoku, x, y);
}

void doSmthToAllConectedCells(int x, int y, struct Sudoku * sud, struct PointerTuple* tupl_arg, void ( *do_func)(int *value, struct PointerTuple* args))
{
    int i = 0, j = 0;
    int sq_start_x = 0, sq_start_y = 0;
    for (i = 0; i < 9; ++i)
    {
        if (i != x)
            do_func(atSudoku(sud, i, y), tupl_arg);
    }
    for (j = 0; j < 9; ++j)
    {
        if (j != y)
            do_func(atSudoku(sud, x, j), tupl_arg);
    }

    sq_start_x = (x / 3) * 3;
    sq_start_y = (x / 3) * 3;

    for (i = sq_start_x; i < sq_start_x + 3; ++i)
    {
        for (j = sq_start_y; j < sq_start_y + 3; ++j)
        {
            //and because cells from same line or column were already processed
            if (i != x && j != y)
                do_func(atSudoku(sud, i, j), tupl_arg);
        }
    }
}

bool hasOnlyOnePossibleVar(bool * possibles)
{
    int i = 0;
    bool meeted_possible = false;
    for (i = 1; i <= 9; ++i)
    {
        if (possibles[i])
        {
            if (meeted_possible)
                return false;
            meeted_possible = possibles[i];
        }
    }
    return meeted_possible;
}


bool isCellWithOnlyPossibleValue(int * val, struct PointerTuple * arg)
{
    int x_proc =
}
bool doSimpleLogicStep(struct Sudoku * sud)
{
    int x_proc = 0, y_proc = 0;
    bool is_only_poss_var = false;
    struct PointerTuple functional_arg = initializePointerTuple(4, (void *)&x_proc, (void *)&y_proc, (void *)sud, (void *)&is_only_poss_var);

    for (x_proc = 0; x_proc < 9; ++x_proc)
    {
        for (y_proc = 0; y_proc < 9; ++y_proc)
        {
            doSmthToAllConectedCells(x_proc, y_proc, sud, 4, &functional_arg, );
        }
    }
}


int main() {

    return 0;
}
