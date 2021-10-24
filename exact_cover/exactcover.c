#include "exactcover.h"
#include "listlikematrix.h"
#include <stdbool.h>
#include <stdio.h>

bool recursivePickOutExactCover(ListLikeMatrix * matrix, VectorInt * result);

bool includingLineDeleteCrissCrossAppleSause(ListLikeMatrix * matrix, ListLikeMatrixLineIterator row)
{
    bool delete_row = false;
    //assuming row is header
    for (ListLikeMatrixLineIterator col = listLikeMatrixIncrementLineIterator(row); !listLikeMatrixIsFinish(col);
            col = listLikeMatrixIncrementLineIterator(col))
    {
        for (ListLikeMatrixColumnIterator downhill_col_it = listLikeMatrixIncrementColumnIterator(listLikeMatrixGetColumnHeader(col));
             !listLikeMatrixIsFinish(downhill_col_it); downhill_col_it = listLikeMatrixIncrementColumnIterator(downhill_col_it))
        {
            delete_row = true;
            if (downhill_col_it == col)
                continue;
            listLikeMatrixDeleteLine(matrix, downhill_col_it);
        }
        listLikeMatrixDeleteColumn(matrix, col);
    }
    // if (delete_raw)
    listLikeMatrixDeleteLine(matrix, row);
    return true;
}




bool checkExactCoverForChoosenRow(ListLikeMatrix * matrix, ListLikeMatrixLineIterator row, VectorInt * result)
{
    bool delete_raw = false;
    listLikeMatrixMakeRestoringLabel(matrix);

    delete_raw = includingLineDeleteCrissCrossAppleSause(matrix, row->line_header);

    if (!delete_raw)
        return false;

    bool res = recursivePickOutExactCover(matrix, result);
    if (!res)
        listLikeMatrixRestoreToLabel(matrix);

    return res;
}

ListLikeMatrixColumnIterator columnWithLeastOnes(ListLikeMatrix * matrix)
{
    int min_ones = listLikeMatrixGetLines(matrix) + 1;
    ListLikeMatrixColumnIterator res = NULL;
    for (ListLikeMatrixColumnIterator it = listLikeMatrixGetColumnBegin(matrix); !listLikeMatrixIsFinish(it);
            it = listLikeMatrixIncrementLineIterator(it))
    {
        int amount = listLikeMatrixGetAmountOfOnesInColumn(matrix, it);
        if (amount == 0)
            return it;
        if (amount < min_ones)
        {
            min_ones = amount;
            res = it;
        }
    }
    return res;
}

bool recursivePickOutExactCover(ListLikeMatrix * matrix, VectorInt * result)
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

VectorInt makeExactCover(const VectorVectorInt * matrix)
{
    ListLikeMatrix exact_cover_matrix = defaultListLikeMatrix(matrix->getSize(matrix), dim(matrix, 2), matrix);
    //*exact_cover_matrix.amount_of_ones_in_columns.at(&exact_cover_matrix.amount_of_ones_in_columns, 0) -= 1;

    VectorInt result = defaultVectorIntCalloc(0, 0);

   // for (int i = 0; i < force_included->getSize(force_included); ++i)
   // {
  //      includingLineDeleteCrissCrossAppleSause(&exact_cover_matrix, getLineOfRealIndex(&exact_cover_matrix, *force_included->cat(force_included, i)));
  //      result.pushBack(&result, *force_included->cat(force_included, i));
   // }

    recursivePickOutExactCover(&exact_cover_matrix, &result);

    destructListLikeMatrix(&exact_cover_matrix);

    sortVectorInt(&result, &intLessComparator);
    return result;

}