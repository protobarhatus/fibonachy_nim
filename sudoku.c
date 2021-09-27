#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <stdarg.h>

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
    memset((void *)arr.arr, def_value, sizeof(bool) * 81 * BOOL_ARR_SIZE);
    return arr;
}
bool * atArray2dBoolArray(struct Array2dBoolArray * obj, int i, int j)
{
    return &obj->arr[(i * obj->w + j) * BOOL_ARR_SIZE];
}


struct PointerTuple
{
    void ** pointers;
    int size;
};

struct PointerTuple constructPointerTuple(int size, ...)
{
    int i = 0;
    va_list pointers;
    va_start(pointers, size);

    struct PointerTuple tuple;
    tuple.size = size;
    tuple.pointers = calloc(size, sizeof(void *));
    for (i = 0; i < size; ++i)
    {
        tuple.pointers[i] = va_arg(pointers, void *);
    }
    va_end(pointers);
    return tuple;
}
void destructPointerTuple(struct PointerTuple * obj)
{
    free(obj->pointers);
}


void * atPointerTuple(struct PointerTuple * obj, int i)
{
    assert(i < obj->size);
    return obj->pointers[i];
}


struct Sudoku
{
    struct Array2d sudoku;
    struct Array2dBoolArray possible_states;
};

void assignValue(int i, int j, int value, struct Sudoku * sud);

bool isDigit(char s)
{
    return s >= '0' && s <= '9';
}

struct Sudoku initializeSudoku(FILE * input)
{
    struct Sudoku obj;
    char c_read = 0, i = 0, j = 0;
    obj.sudoku = defaultArray2d(9, 9);
    obj.possible_states = defaultArray2dBoolArray(9, 9, true);
   // printf("start loading\n");
    for (i = 0; i < 9; ++i)
    {
        for (j = 0; j < 9; ++j)
        {
           // printf("%d %d\n", i, j);
            do {
                fscanf(input, "%c", &c_read);
            } while (!isDigit(c_read));
            assignValue(i, j, c_read - '0', &obj);
        }
    }
    fgetc(input);
   // printf("end loading\n");
    return obj;

}

int *atSudoku(struct Sudoku * obj, int i, int j)
{
    return atArray2d(&obj->sudoku, i, j);
}

bool isCellUndefined(int i, int j, struct Sudoku * sud)
{
    return *atSudoku(sud, i, j) == 0;
}

bool isComplete(struct Sudoku * obj)
{
    int i = 0;
    int j = 0;
    for (i = 0; i < obj->sudoku.w; ++i)
    {
        for (j = 0; j < obj->sudoku.h; ++j)
            if (isCellUndefined(i, j, obj))
                return false;
    }
    return true;
}


void outSudoku(struct Sudoku * obj, FILE * output)
{
    int i = 0, j = 0;
    for (i = 0; i < 9; ++i)
    {
        for (j = 0; j < 9; ++j)
        {
            fprintf(output, "%d ", *atSudoku(obj, i, j));
        }
        fprintf(output, "\n");
    }
}


void doSmthToAllConnectedCells(int i_cur, int j_cur, struct Sudoku * sud, struct PointerTuple* tupl_arg,
                               void ( *do_func)(int i, int j, int val, struct PointerTuple* args))
{
    int i = 0, j = 0;
    int sq_start_i = 0, sq_start_j = 0;
    for (i = 0; i < 9; ++i)
    {
        if (i != i_cur)
            do_func(i, j_cur, *atSudoku(sud, i, j_cur), tupl_arg);
    }
    for (j = 0; j < 9; ++j)
    {
        if (j != j_cur)
            do_func(i_cur, j, *atSudoku(sud, i_cur, j), tupl_arg);
    }

    sq_start_i = (i_cur / 3) * 3;
    sq_start_j = (j_cur / 3) * 3;

    for (i = sq_start_i; i < sq_start_i + 3; ++i)
    {
        for (j = sq_start_j; j < sq_start_j + 3; ++j)
        {
            //and is because cells from same line or column were already processed
            if (i != i_cur && j != j_cur)
                do_func(i, j, *atSudoku(sud, i, j), tupl_arg);
        }
    }
}

int hasOnlyOnePossibleVar(const bool * possibles)
{
    int i = 0;
    int val = 0;
    for (i = 1; i <= 9; ++i)
    {
        if (possibles[i])
        {
            if (val)
                return 0;
            val = possibles[i] ? i : val;
        }
    }
    return val;
}

int amountOfPossibleVars(const bool * possibles)
{
    int amount = 0, i = 0;
    for (i = 1; i < 10; ++i)
        if (possibles[i])
            ++amount;
    return amount;
}

void unsetPossibleValue(int i, int j, int cell_val, struct PointerTuple * args)
{
    struct Array2dBoolArray * possibles = (struct Array2dBoolArray*) atPointerTuple(args, 0);
    int val = *(int*) atPointerTuple(args, 1);
    atArray2dBoolArray(possibles, i, j)[val] = false;
}
void assignValue(int i, int j, int value, struct Sudoku * sud)
{
    *atArray2d(&sud->sudoku, i, j) = value;
  //printf("pointer: %i\n", &sud->possible_states);
    struct PointerTuple arg = constructPointerTuple(2, (void *) &sud->possible_states, (void *)&value);
    doSmthToAllConnectedCells(i, j, sud, &arg, unsetPossibleValue);
    destructPointerTuple(&arg);
}
bool doSimpleLogicStep(struct Sudoku * sud)
{
    int i_proc = 0, j_proc = 0;
    int value = 0;

    for (i_proc = 0; i_proc < 9; ++i_proc)
    {
        for (j_proc = 0; j_proc < 9; ++j_proc)
        {

            if (isCellUndefined(i_proc, j_proc, sud) &&
                (value = hasOnlyOnePossibleVar(atArray2dBoolArray(&sud->possible_states, i_proc, j_proc))))
            {
                assignValue(i_proc, j_proc, value, sud);
                return true;
            }
        }
    }
    return false;
}

bool trySimpleLogicApproach(struct Sudoku * sud)
{
    bool did_something = false;
    while (doSimpleLogicStep(sud))
        did_something = true;
    return did_something;
}

struct Pair
{
    int first, second;
};

struct Pair pair(int a, int b)
{
    struct Pair p = {a, b};
    return p;
}

struct Pair cellWithMinAmountOfPossibles(struct Sudoku * sudoku)
{
    int min_amount = 10;
    int loc_amount = 0;
    int i = 0, j = 0;
    struct Pair res = {-1, -1};
    for (i = 0; i < sudoku->sudoku.w; ++i)
    {
        for (j = 0; j < sudoku->sudoku.h; ++j)
        {
            if (!isCellUndefined(i, j, sudoku))
                continue;
            loc_amount = amountOfPossibleVars(atArray2dBoolArray(&sudoku->possible_states, i, j));
            if (loc_amount < min_amount && loc_amount > 0)
            {
                min_amount = loc_amount;
                res = pair(i, j);
            }
        }
    }
    return res;

}

bool completeSudoku(struct Sudoku * sudoku);

bool tryVariants(struct Sudoku * sudoku)
{
    struct Pair cell = cellWithMinAmountOfPossibles(sudoku);
    if (cell.first == -1)
        return false;

    bool * its_possibles = atArray2dBoolArray(&sudoku->possible_states, cell.first, cell.second);
    int i = 0;
    struct Sudoku variants[10] = {{}};
    int sud_counter = 0;
    for (i = 1; i < 10; ++i)
    {
        if (its_possibles[i])
        {
            variants[sud_counter] = *sudoku;
            assignValue(cell.first, cell.second, i, &variants[sud_counter]);
            ++sud_counter;
        }
    }
    if (sud_counter == 0)
        return false;
    for (i = 0; i < sud_counter; ++i)
    {
        if (completeSudoku(&variants[i]))
        {
            *sudoku = variants[i];
            return true;
        }
    }
    return false;
}

bool completeSudoku(struct Sudoku * sudoku)
{
    trySimpleLogicApproach(sudoku);
    if (!isComplete(sudoku))
        return tryVariants(sudoku);
    return true;
}

void checkInequality(int i_cell, int j_cell, int cur_value, struct PointerTuple * args)
{
    int value = *(int *)atPointerTuple(args, 0);
    bool * result_write = (bool *) atPointerTuple(args, 1);

    if (value == cur_value)
        *result_write = true;
}

bool checkSudoku(struct Sudoku * sudoku)
{
    int i = 0, j = 0;
    bool has_equal_connected_el = false;
    int local_value = 0;
    struct PointerTuple args = constructPointerTuple(2, &local_value, &has_equal_connected_el);

    for (i = 0; i < sudoku->sudoku.w; ++i)
    {
        for (j = 0; j < sudoku->sudoku.h; ++j)
        {
            if (isCellUndefined(i, j, sudoku))
                return false;
            has_equal_connected_el = false;
            local_value = *atSudoku(sudoku, i, j);
            doSmthToAllConnectedCells(i, j, sudoku, &args, checkInequality);
            if (has_equal_connected_el)
                return false;
        }
    }
    destructPointerTuple(&args);

    return true;
}

int main()
{
    setbuf(stdout, 0);
    struct Sudoku sudoku;
    bool found_res = false;
    bool res_right = false;
    char prestr[100];
    int sum = 0;
    while (true)
    {
        printf("Ent:\n");
        gets(prestr);
        if (prestr[0] == 'S')
            printf("%d\n", sum);
        printf("||| %s |||||", prestr);
        sudoku = initializeSudoku(stdin);
        found_res = completeSudoku(&sudoku);
        if (!found_res)
            printf("ERROR: didn't found solution\n");
        res_right = checkSudoku(&sudoku);
        if (!res_right)
        {
            printf("ERROR: wrong res\n");
            outSudoku(&sudoku, stdout);
        }
        if (found_res && res_right)
            printf("Succesfully\n");
        sum += *atSudoku(&sudoku, 0, 0) * 100 + *atSudoku(&sudoku, 0, 1) * 10 + *atSudoku(&sudoku, 0, 2);
    }


    /*struct Sudoku sudoku = initializeSudoku(stdin);

    printf("Loaded\n");
    completeSudoku(&sudoku);

    outSudoku(&sudoku, stdout);*/
    return 0;
}