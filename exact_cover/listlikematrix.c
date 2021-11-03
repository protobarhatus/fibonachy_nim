#include "listlikematrix.h"
#include <stdlib.h>

static void assignFunctions(ListLikeMatrix * matrix)
{
   // matrix->atm = &atListLikeMatrix;
  //  matrix->catm = &catListLikeMatrix;
    matrix->getValue = &listLikeMatrixGetValue;
    matrix->getColumns = &listLikeMatrixGetColumns;
    matrix->getLines = &listLikeMatrixGetLines;
    matrix->deleteColumn = &listLikeMatrixDeleteColumn;
    matrix->deleteLine = &listLikeMatrixDeleteLine;
    matrix->iterateOnLine = &listLikeMatrixIncrementLineIterator;
    matrix->iterateOnColumn = &listLikeMatrixIncrementColumnIterator;
    matrix->restoreLine = &listLikeMatrixRestoreLine;
    matrix->restoreColumn = &listLikeMatrixRestoreColumn;
    matrix->linesBegin = &listLikeMatrixGetLineBegin;
    matrix->columnsBegin = &listLikeMatrixGetColumnBegin;
    matrix->linesEnd = &listLikeMatrixGetLineEnd;
    matrix->columnsEnd = &listLikeMatrixGetColumnEnd;
    matrix->isFinish = &listLikeMatrixIsFinish;
    matrix->getColumnHeader = &listLikeMatrixGetColumnHeader;
    matrix->getLineHeader = &listLikeMatrixGetLineHeader;
    matrix->makeRestoringLabel = &listLikeMatrixMakeRestoringLabel;
    matrix->restore = &listLikeMatrixRestoreToLabel;
    matrix->getAmountOfOnesInColumn = &listLikeMatrixGetAmountOfOnesInColumn;
    matrix->decrementOnColumn = &listLikeMatrixDecrementColumnIterator;
    matrix->addLine = &listLikeMatrixAddLine;
}

static bool hasOne(const VectorInt * vec)
{
    for (int i = 0; i < vec->getSize(vec); ++i)
        if (*vec->cat(vec, i) == 1)
            return true;
    return false;
}
#define CALLOC_DUM_P 0
static void createLinesAndColumnsFinishersVectors(ListLikeMatrix * mat)
{
    mat->lines_finishers = defaultVectorListLikeMatrixNodePtrCalloc(mat->lines + 1, CALLOC_DUM_P);
    mat->columns_finishers = defaultVectorListLikeMatrixNodePtrCalloc(mat->columns + 1, CALLOC_DUM_P);

    *mat->lines_finishers.at(&mat->lines_finishers, mat->lines) = callocDefaultListLikeMatrixNode(-1, 1e9, *mat->columns_headers.back(&mat->columns_headers), NULL);
    *mat->columns_finishers.at(&mat->columns_finishers, mat->columns) = callocDefaultListLikeMatrixNode(1e9, -1, NULL, *mat->lines_headers.back(&mat->lines_headers));
}
static void createAndFillHeaders(ListLikeMatrix * res, int lines, int columns)
{
    res->columns_headers = defaultVectorListLikeMatrixNodePtrCalloc(0, CALLOC_DUM_P);
    res->lines_headers = defaultVectorListLikeMatrixNodePtrCalloc(0, CALLOC_DUM_P);
    res->columns_headers.pushBack(&res->columns_headers, callocDefaultListLikeMatrixNode(-1, 0, NULL, NULL));
    res->lines_headers.pushBack(&res->lines_headers, callocDefaultListLikeMatrixNode(0, -1, NULL, NULL));

    for (int i = 1; i < columns; ++i)
        res->columns_headers.pushBack(&res->columns_headers, callocDefaultListLikeMatrixNode(-1, i,
                                                          *res->columns_headers.at(&res->columns_headers, i - 1), NULL));

    for (int i = 1; i < lines; ++i)
        res->lines_headers.pushBack(&res->lines_headers, callocDefaultListLikeMatrixNode(i, -1, NULL, *res->lines_headers.at(&res->lines_headers, i - 1)));
}

static void assignBasicValues(ListLikeMatrix * res, int lines, int columns, const VectorVectorInt * orig_matrix)
{
    assignFunctions(res);
    res->matrix_replace = orig_matrix;

    res->columns = columns;
    res->lines = lines;
    res->absolute_lines = lines;
    res->absolute_columns = columns;
    res->lines_start = 0;
    res->columns_start = 0;
    res->maintain_deleting_and_restoring_order = true;

    res->deleting_labels = defaultVectorIntCalloc(0, 0);
    res->deleting_log = defaultVectorPairIntCalloc(0, defaultPairInt(0, 0));

}

static void fillLineSAndColumnsFinishers(ListLikeMatrix * mat, int lines, int columns, VectorListLikeMatrixNodeRef * upper_columns)
{
    for (int i = 0; i < lines; ++i)
    {
        if (mat->matrix.at(&mat->matrix, i)->getSize(mat->matrix.at(&mat->matrix, i)) > 0)
            *mat->lines_finishers.at(&mat->lines_finishers, i) = callocDefaultListLikeMatrixNode(i, 1e9, *mat->matrix.at(&mat->matrix, i)->back(mat->matrix.at(&mat->matrix, i)), NULL);
        else
            *mat->lines_finishers.at(&mat->lines_finishers, i) = callocDefaultListLikeMatrixNode(i, 1e9, *mat->lines_headers.at(&mat->lines_headers, i), NULL);
    }

    for (int i = 0; i < columns; ++i)
        *mat->columns_finishers.at(&mat->columns_finishers, i) = callocDefaultListLikeMatrixNode(1e9, i, NULL, *upper_columns->at(upper_columns, i));
}

static void countAmountOfOnesOnColumns(ListLikeMatrix * mat)
{
    mat->amount_of_ones_in_columns = defaultVectorIntCalloc(mat->columns, 0);
    for (int i = 0; i < mat->columns; ++i)
    {
        int num = 0;
        for (ListLikeMatrixColumnIterator it = mat->iterateOnColumn(mat->columnsBegin(mat)); !mat->isFinish(it); it = mat->iterateOnColumn(it))
            ++num;
        *mat->amount_of_ones_in_columns.at(&mat->amount_of_ones_in_columns, i) = num;
    }
}

static void createMainMatrixAndFinishers(ListLikeMatrix * res, int lines, int columns, const VectorVectorInt *  orig_matrix)
{
    VectorListLikeMatrixNodeRef upper_nodes_in_column = defaultVectorListLikeMatrixNodeRefCalloc(columns, 0);
    for (int i = 0; i < columns; ++i)
        *upper_nodes_in_column.at(&upper_nodes_in_column, i) = *res->columns_headers.at(&res->columns_headers, i);

    int lines_counter = 0;
    for (int i = 0; i < lines; ++i)
    {
        int added_columns_counter = 0;
        //  if (!hasOne(orig_matrix->cat(orig_matrix, i)))
        //    continue;

        res->matrix.pushBack(&res->matrix, defaultVectorListLikeMatrixNodePtrCalloc(0, 0));
        for (int j = 0; j < columns; ++j)
        {
            if (*orig_matrix->cat(orig_matrix, i, j) != 1)
                continue;
            ListLikeMatrixNode * pr_on_line = (added_columns_counter == 0 ?
                                               *res->lines_headers.at(&res->lines_headers, i) : *res->matrix.at(&res->matrix, lines_counter, added_columns_counter - 1));
            ListLikeMatrixNode * pr_on_column = *upper_nodes_in_column.at(&upper_nodes_in_column, j);

            res->matrix.back(&res->matrix)->pushBack(res->matrix.back(&res->matrix),
                                                   callocDefaultListLikeMatrixNode(i, j, pr_on_line, pr_on_column));
            ++added_columns_counter;
            //*last_lines_of_ones_in_column.at(&last_lines_of_ones_in_column, j) = lines_counter;
            *upper_nodes_in_column.at(&upper_nodes_in_column, j) = *res->matrix.back(&res->matrix)->back(res->matrix.back(&res->matrix));
        }
        ++lines_counter;
    }
    fillLineSAndColumnsFinishers(res, lines, columns, &upper_nodes_in_column);
    destructVectorListLikeMatrixNodeRef(&upper_nodes_in_column);
}


ListLikeMatrix defaultListLikeMatrix(int lines, int columns, const VectorVectorInt * orig_matrix)
{
    ListLikeMatrix res;
    assignBasicValues(&res, lines, columns, orig_matrix);

    res.matrix = defaultVectorVectorListLikeMatrixNodePtrCalloc(0, defaultVectorListLikeMatrixNodePtrCalloc(columns, NULL));

    createAndFillHeaders(&res, lines, columns);

    createLinesAndColumnsFinishersVectors(&res);
   // VectorInt last_lines_of_ones_in_column = defaultVectorIntCalloc(columns, -1);
    createMainMatrixAndFinishers(&res, lines, columns, orig_matrix);

    listLikeMatrixAssignFinishers(&res);
    countAmountOfOnesOnColumns(&res);
    return res;
}

int listLikeMatrixGetValue(ListLikeMatrix * mat, int l, int c)
{
    assert(l < mat->getLines(mat));
    assert(c < mat->getColumns(mat));
    ListLikeMatrixColumnIterator col_h = mat->columnsBegin(mat);
    ListLikeMatrixLineIterator lin_h = mat->linesBegin(mat);
    for (int i = 0; i < l; ++i)
        lin_h = mat->iterateOnLine(lin_h);
    for (int i = 0; i < c; ++c)
        col_h = mat->iterateOnColumn(col_h);
    while (lin_h->column < col_h->column)
        lin_h = mat->iterateOnColumn(lin_h);
    if (lin_h->column == col_h->column)
        return 1;
    return 0;
}

ListLikeMatrixColumnIterator getColumnStart(ListLikeMatrixAnyIterator it)
{
    if (it->column_header != NULL)
        return it->column_header->next_on_column;
    return it->next_on_column;
}

/*VectorVectorInt * getMatrixArray(ListLikeMatrix * mat)
{
    return mat->matrix_replace;
}*/
const VectorVectorInt * getMatrixArrayConst(const ListLikeMatrix * max)
{
    return max->matrix_replace;
}



void listLikeMatrixDeleteLine(ListLikeMatrix * mat, ListLikeMatrixLineIterator line)
{
    if (!line->line_header->active)
        return;
    if (line->line_header->prev_on_column == NULL)
        mat->lines_start = line->line_header->next_on_column->line;
    for (ListLikeMatrixLineIterator it = line->line_header; !listLikeMatrixIsFinish(it);
            it = listLikeMatrixIncrementLineIterator(it))
    {
        eraseListLikeMatrixNodeFromColumn(it);
        if (it->column >= 0)
            *atVectorInt(&mat->amount_of_ones_in_columns, it->column) -= 1;
    }
    mat->lines--;

    vectorPairIntPushBack(&mat->deleting_log, defaultPairInt(DELETING_TYPE_LINE, line->line));
}
void listLikeMatrixDeleteColumn(ListLikeMatrix * mat, ListLikeMatrixColumnIterator column)
{
    if (!column->column_header->active)
        return;
    if (column->column_header->prev_on_line == NULL)
        mat->columns_start = column->column_header->next_on_line->column;
  //  for (ListLikeMatrixColumnIterator it = column->column_header; !mat->isFinish(it); it = mat->iterateOnColumn(it))
   //     eraseListLikeMatrixNodeFromLine(it);
    eraseListLikeMatrixNodeFromLine(column->column_header);
    mat->columns--;

    vectorPairIntPushBack(&mat->deleting_log, defaultPairInt(DELETING_TYPE_COLUMN, column->column));
}

ListLikeMatrixLineIterator getLineOfRealIndex(ListLikeMatrix * mat, int index)
{
    return *mat->lines_headers.at(&mat->lines_headers, index);
}


void listLikeMatrixRestoreColumnNode(ListLikeMatrix * mat, ListLikeMatrixColumnIterator it)
{
    it->active = true;
    if (it->prev_on_line == NULL)
    {
        assert(it->next_on_line->prev_on_line == NULL);
        it->next_on_line->prev_on_line = it;
    }
    else
    {
        assert(it->next_on_line->prev_on_line == it->prev_on_line);
        assert(it->prev_on_line->next_on_line == it->next_on_line);
        it->next_on_line->prev_on_line = it;
        it->prev_on_line->next_on_line = it;
    }

  //  mat->deleting_log.popBack(&mat->deleting_log);
}
void listLikeMatrixRestoreColumn(ListLikeMatrix * mat, ListLikeMatrixColumnIterator it)
{
    mat->columns++;
    if (mat->columns_start > it->column)
        mat->columns_start = it->column;
  //  for (ListLikeMatrixColumnIterator iter = it->column_header; !mat->isFinish(iter); iter = mat->iterateOnColumn(iter))
  //      listLikeMatrixRestoreColumnNode(mat, iter);
    listLikeMatrixRestoreColumnNode(mat, it->column_header);
}
void listLikeMatrixRestoreLineNode(ListLikeMatrix * mat, ListLikeMatrixLineIterator it)
{
    it->active = true;
    if (it->prev_on_column == NULL)
    {
        assert(it->next_on_column->prev_on_column == NULL);
        it->next_on_column->prev_on_column = it;
    }
    else
    {
        assert(it->next_on_column->prev_on_column == it->prev_on_column);
        assert(it->prev_on_column->next_on_column == it->next_on_column);
        it->next_on_column->prev_on_column = it;
        it->prev_on_column->next_on_column = it;
    }

    //mat->deleting_log.popBack(&mat->deleting_log);
}
void listLikeMatrixRestoreLine(ListLikeMatrix * mat, ListLikeMatrixLineIterator it)
{
    mat->lines++;
    if (mat->lines_start > it->line)
        mat->lines_start = it->line;
    for (ListLikeMatrixColumnIterator iter = it->line_header; !listLikeMatrixIsFinish(iter);
            iter = listLikeMatrixIncrementLineIterator(iter))
    {
        listLikeMatrixRestoreLineNode(mat, iter);
        if (iter->column >= 0)
            *atVectorInt(&mat->amount_of_ones_in_columns, iter->column) += 1;
    }
}








void destructListLikeMatrix(ListLikeMatrix * obj)
{
    //since it UNIQUE_PTR, they free data behind that pointers themselves
    destructVectorVectorListLikeMatrixNodePtr(&obj->matrix);
    destructVectorListLikeMatrixNodePtr(&obj->lines_headers);
    destructVectorListLikeMatrixNodePtr(&obj->columns_headers);
    destructVectorListLikeMatrixNodePtr(&obj->lines_finishers);
    destructVectorListLikeMatrixNodePtr(&obj->columns_finishers);
    destructVectorInt(&obj->amount_of_ones_in_columns);
    destructVectorPairInt(&obj->deleting_log);
    destructVectorInt(&obj->deleting_labels);
}

int getRealLineIndex(ListLikeMatrixColumnIterator it)
{
    return it->line;
}






//so nodes should be calloced in strict order after headers
ListLikeMatrixNode* callocDefaultListLikeMatrixNode(int line, int column, ListLikeMatrixNode * prev_on_line, ListLikeMatrixNode * prev_on_column)
{
    ListLikeMatrixNode* result = (ListLikeMatrixNode *)calloc(1, sizeof(ListLikeMatrixNode));
    result->line = line;
    result->column = column;
    result->prev_on_line = prev_on_line;
    result->prev_on_column = prev_on_column;
    result->active = true;

    if (prev_on_line != NULL)
    {
        prev_on_line->next_on_line = result;
        result->line_header = prev_on_line->line_header;
    }
    else
        result->line_header = result;
    if (prev_on_column != NULL)
    {
        prev_on_column->next_on_column = result;
        result->column_header = prev_on_column->column_header;
    }
    else
        result->column_header = result;


    return result;
}

void eraseListLikeMatrixNodeFromLine(ListLikeMatrixNode * node)
{
    if (node->prev_on_line == NULL)
    {
        node->next_on_line->prev_on_line = NULL;
    }
    else
    {
        node->prev_on_line->next_on_line = node->next_on_line;
        node->next_on_line->prev_on_line = node->prev_on_line;
    }
    node->active = false;

}
void eraseListLikeMatrixNodeFromColumn(ListLikeMatrixNode * node)
{
    if (node->prev_on_column == NULL)
    {
        node->next_on_column->prev_on_column = NULL;
    }
    else
    {
        node->prev_on_column->next_on_column = node->next_on_column;
        node->next_on_column->prev_on_column = node->prev_on_column;
    }
    node->active = false;
}
void restoreListLikeMatrixNodeToLine(ListLikeMatrixNode * node)
{
    if (node->prev_on_line != NULL)
        node->prev_on_line->next_on_line = node;
    node->next_on_line->prev_on_line = node;
    node->active = true;
}
void restoreListLikeMatrixNodeToColumn(ListLikeMatrixNode * node)
{
    if (node->prev_on_column != NULL)
        node->prev_on_column->next_on_column = node;
    node->next_on_column->prev_on_column = node;
    node->active = true;
}




int getLineOfIterator(ListLikeMatrixAnyIterator it)
{
    return it->line;
}

void listLikeMatrixAssignFinishers(ListLikeMatrix * mat)
{
    for (int i = 0; i < mat->absolute_lines + 1; ++i)
    {
        ListLikeMatrixLineIterator it = (*mat->lines_finishers.at(&mat->lines_finishers, i))->prev_on_line;
        while (it != NULL)
        {
            it->line_finish = *mat->lines_finishers.at(&mat->lines_finishers, i);
            it = it->prev_on_line;
        }
    }
    for (int i = 0; i < mat->absolute_columns + 1; ++i)
    {
        ListLikeMatrixColumnIterator it = (*mat->columns_finishers.at(&mat->columns_finishers, i))->prev_on_column;
        while (it != NULL)
        {
            it->column_finish = *mat->columns_finishers.at(&mat->columns_finishers, i);
            it = it->prev_on_column;
        }
    }

}






void listLikeMatrixRestoreToLabel(ListLikeMatrix * mat)
{
    assert(mat->maintain_deleting_and_restoring_order);

    if (vectorIntGetSize(&mat->deleting_labels) == 0)
        return;
    int last_label = *vectorIntBack(&mat->deleting_labels);
    int size = vectorPairIntGetSize(&mat->deleting_log);
    int diff = size - last_label;
    for (int i = 0; i < diff; ++i)
    {
        PairInt del = *atVectorPairInt(&mat->deleting_log, size - 1 - i);
        if (del.first == DELETING_TYPE_LINE)
            listLikeMatrixRestoreLine(mat, *atVectorListLikeMatrixNodePtr(&mat->lines_headers, del.second));
        else if (del.first == DELETING_TYPE_COLUMN)
            listLikeMatrixRestoreColumn(mat, *atVectorListLikeMatrixNodePtr(&mat->columns_headers, del.second));
        else
            assert(false);

    }
    vectorIntPopBack(&mat->deleting_labels);
    vectorPairIntResize(&mat->deleting_log, last_label);

}
void listLikeMatrixAddLine(ListLikeMatrix * mat, ListLikeMatrixColumnIterator iterc)
{
    assert(false);
    ListLikeMatrixColumnIterator iter = iterc->line_header;
    mat->maintain_deleting_and_restoring_order = false;
    ListLikeMatrixColumnIterator prev_active;
    ListLikeMatrixColumnIterator next_active;
    for (prev_active = iter; prev_active != NULL && !prev_active->active ;prev_active = mat->decrementOnColumn(prev_active))
        ;
    for (next_active = iter; !next_active->active;next_active = mat->iterateOnColumn(next_active))
        ;

    iter->active = true;
    iter->next_on_column = next_active;
    iter->prev_on_column = prev_active;
    next_active->prev_on_column = iter;
    if (prev_active != NULL)
        prev_active->next_on_column = iter;
    ++mat->lines;
    if (prev_active == NULL)
        mat->lines_start = iter->line;

}

