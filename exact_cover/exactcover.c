#include "exactcover.h"
#include "listlikematrix.h"
#include <stdbool.h>
#include <stdio.h>

bool recursivePickOutExactCover(ListLikeMatrix * matrix, VectorInt * result);

bool includingLineDeleteCrissCrossAppleSause(ListLikeMatrix * matrix, ListLikeMatrixLineIterator row)
{
    bool delete_row = false;
    //assuming row is header
    for (ListLikeMatrixLineIterator col = matrix->iterateOnLine(row); !matrix->isFinish(col); col = matrix->iterateOnLine(col))
    {
        for (ListLikeMatrixColumnIterator downhill_col_it = matrix->iterateOnColumn(matrix->getColumnHeader(col));
             !matrix->isFinish(downhill_col_it); downhill_col_it = matrix->iterateOnColumn(downhill_col_it))
        {
            delete_row = true;
            if (downhill_col_it == col)
                continue;
            matrix->deleteLine(matrix, downhill_col_it);
        }
        matrix->deleteColumn(matrix, col);
    }
    // if (delete_raw)
    matrix->deleteLine(matrix, row);
    return delete_row;
}

bool checkExactCoverForChoosenRow(ListLikeMatrix * matrix, ListLikeMatrixLineIterator row, VectorInt * result)
{
    bool delete_raw = false;
    matrix->makeRestoringLabel(matrix);

    delete_raw = includingLineDeleteCrissCrossAppleSause(matrix, row);

    if (!delete_raw)
        return false;

    bool res = recursivePickOutExactCover(matrix, result);
    if (!res)
        matrix->restore(matrix);

    return res;
}

ListLikeMatrixColumnIterator columnWithLeastOnes(ListLikeMatrix * matrix)
{
    int min_ones = matrix->getLines(matrix) + 1;
    ListLikeMatrixColumnIterator res = NULL;
    for (ListLikeMatrixColumnIterator it = matrix->columnsBegin(matrix); !matrix->isFinish(it); it = matrix->iterateOnLine(it))
    {
        int amount = matrix->getAmountOfOnesInColumn(matrix, it);
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
    int lines = matrix->getLines(matrix);
    for (ListLikeMatrixColumnIterator it = matrix->iterateOnColumn(matrix->getColumnHeader(c)); !matrix->isFinish(it); it = matrix->iterateOnColumn(it))
    {
        if (checkExactCoverForChoosenRow(matrix, matrix->getLineHeader(it), result))
        {
            result->pushBack(result, it->line);
            return true;
        }
    }
    return false;


}

VectorInt makeExactCover(const VectorVectorInt * matrix, const VectorInt * force_included)
{
    ListLikeMatrix exact_cover_matrix = defaultListLikeMatrix(matrix->getSize(matrix), dim(matrix, 2), matrix);
    //*exact_cover_matrix.amount_of_ones_in_columns.at(&exact_cover_matrix.amount_of_ones_in_columns, 0) -= 1;

    VectorInt result = defaultVectorIntCalloc(0, 0);

    for (int i = 0; i < force_included->getSize(force_included); ++i)
    {
        includingLineDeleteCrissCrossAppleSause(&exact_cover_matrix, getLineOfRealIndex(&exact_cover_matrix, *force_included->cat(force_included, i)));
        result.pushBack(&result, *force_included->cat(force_included, i));
    }

    recursivePickOutExactCover(&exact_cover_matrix, &result);

    destructListLikeMatrix(&exact_cover_matrix);
    return result;

}