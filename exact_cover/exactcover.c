#include "exactcover.h"
#include "listlikematrix.h"
#include <stdbool.h>
#include <stdio.h>

/* exact cover is is solved by algorithm X:
 * makeExactCover is interface wrapping that sets all necessary structures like translates simple matrix to ListLikeMatrix (that is sparse matrix)
 * and call actual algorithm:
 * in recursivePickOutExactCover it chooses column with least amount of ones in it
 * and goes through all rows, calling checkExactCoverForChoosenRow() for each of them
 * if checkExactCoverForChoosenRow() returns successfully then this row is chosen right and recursivePickOutExactCover can return successfully
 * In checkExactCoverForChoosenRow() it saves current matrix state (by making label),
 * deletes (in deleteAllContradictoryRowsAndColumnsForChoosenLine)
 * all columns where chosen row I has ones, and all rows that have ones in that columns and
 * launches algorithm recursively on reduced matrix. if its successful, returns success, otherwise restores matrix and terminates this line
 *
 */

static bool recursivePickOutExactCover(ListLikeMatrix * matrix, VectorInt * result);

static void deleteAllContradictoryRowsAndColumnsForChoosenLine(ListLikeMatrix * matrix, ListLikeMatrixLineIterator row)
{
    //assuming row is header
    for (ListLikeMatrixLineIterator col = listLikeMatrixIncrementLineIterator(row); !listLikeMatrixIsFinish(col);
            col = listLikeMatrixIncrementLineIterator(col))
    {
        for (ListLikeMatrixColumnIterator downhill_col_it = listLikeMatrixIncrementColumnIterator(listLikeMatrixGetColumnHeader(col));
             !listLikeMatrixIsFinish(downhill_col_it); downhill_col_it = listLikeMatrixIncrementColumnIterator(downhill_col_it))
        {
            if (downhill_col_it == col)
                continue;
            listLikeMatrixDeleteLine(matrix, downhill_col_it);
        }
        listLikeMatrixDeleteColumn(matrix, col);
    }
    listLikeMatrixDeleteLine(matrix, row);
}




static bool checkExactCoverForChoosenRow(ListLikeMatrix * matrix, ListLikeMatrixLineIterator row, VectorInt * result)
{
    listLikeMatrixMakeRestoringLabel(matrix);

    deleteAllContradictoryRowsAndColumnsForChoosenLine(matrix, row->line_header);


    bool res = recursivePickOutExactCover(matrix, result);
    if (!res)
        listLikeMatrixRestoreToLabel(matrix);

    return res;
}

static ListLikeMatrixColumnIterator columnWithLeastOnes(ListLikeMatrix * matrix)
{
    int min_ones = listLikeMatrixGetLines(matrix) + 1;
    ListLikeMatrixColumnIterator res = NULL;
    for (ListLikeMatrixColumnIterator it = listLikeMatrixGetColumnBegin(matrix); !listLikeMatrixIsFinish(it);
            it = listLikeMatrixIncrementLineIterator(it))
    {
        int amount = listLikeMatrixGetAmountOfOnesInColumn(matrix, it);
        if (amount == 0)
            return it;
        if (amount <= min_ones)
        {
            min_ones = amount;
            res = it;
        }
    }
    return res;
}

static bool recursivePickOutExactCover(ListLikeMatrix * matrix, VectorInt * result)
{
    if (listLikeMatrixGetColumns(matrix) == 0)
        return true;
    ListLikeMatrixColumnIterator c = columnWithLeastOnes(matrix);
    if (c == NULL)
        return true;
    int new_lines_stack_start = 0;

    for (ListLikeMatrixColumnIterator it = listLikeMatrixIncrementColumnIterator(listLikeMatrixGetColumnHeader(c));
            !listLikeMatrixIsFinish(it); it = listLikeMatrixIncrementColumnIterator(it))
    {
        if (checkExactCoverForChoosenRow(matrix, it, result))
        {
            result->pushBack(result, it->line);
            return true;
        }
    }

    return false;


}

static bool hasEmptyColumn(const VectorVectorInt * matrix)
{
    int width = dim(matrix, 2);
    int height = matrix->getSize(matrix);
    for (int i = 0; i < width; ++i)
    {
        bool has_non_zero = false;
        for (int j = 0; j < height; ++j)
        {
            if (*matrix->cat(matrix, j, i) != 0)
            {
                has_non_zero = true;
                break;
            }
        }
        if (!has_non_zero)
            return true;
    }
    return false;
}

VectorInt makeExactCover(const VectorVectorInt * matrix)
{
    //columns consisting entirely of zeroes will be just ignored by ListLikeMatrix so answer would be wrong, so need to check them manually
    if (hasEmptyColumn(matrix))
        return defaultVectorIntCalloc(0, 0);
    ListLikeMatrix exact_cover_matrix = defaultListLikeMatrix(matrix->getSize(matrix), dim(matrix, 2), matrix);

    VectorInt result = defaultVectorIntCalloc(0, 0);


    recursivePickOutExactCover(&exact_cover_matrix, &result);

    destructListLikeMatrix(&exact_cover_matrix);

    sortVectorInt(&result, &intLessComparator);
    return result;

}