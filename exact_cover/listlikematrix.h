#ifndef EXACT_COVER_LISTLIKEMATRIX_H
#define EXACT_COVER_LISTLIKEMATRIX_H
#include "vector.h"
#include "algorithms.h"

struct ListLikeMatrixNode_struct
{
    int line, column;
    struct ListLikeMatrixNode_struct * next_on_line, * next_on_column, *prev_on_line, *prev_on_column;
    //column start is not defined for headers and finishers so pointers are NULL
    struct ListLikeMatrixNode_struct * column_header, *line_header;

    struct ListLikeMatrixNode_struct * line_finish, *column_finish;

    bool active;
};
typedef struct ListLikeMatrixNode_struct ListLikeMatrixNode;
typedef ListLikeMatrixNode* ListLikeMatrixLineIterator;
typedef ListLikeMatrixNode* ListLikeMatrixColumnIterator;
typedef ListLikeMatrixNode* ListLikeMatrixAnyIterator;

int getLineOfIterator(ListLikeMatrixAnyIterator it);


ListLikeMatrixNode* callocDefaultListLikeMatrixNode(int line, int column, ListLikeMatrixNode * prev_on_line, ListLikeMatrixNode * prev_on_column);
void eraseListLikeMatrixNodeFromLine(ListLikeMatrixNode * node);
void eraseListLikeMatrixNodeFromColumn(ListLikeMatrixNode * node);

MAKE_VECTOR(ListLikeMatrixNode*, ListLikeMatrixNodeP, POINTER_TYPE(ListLikeMatrixNodeP));
MAKE_VECTOR(ListLikeMatrixNode*, ListLikeMatrixNodeRef, SIMPLE_TYPE);

MAKE_VECTOR(VectorListLikeMatrixNodeP, VectorListLikeMatrixNodeP, STRUCT_TYPE(VectorListLikeMatrixNodeP))

enum DeletingType
{
    DELETING_TYPE_LINE,
    DELETING_TYPE_COLUMN
};

//tradeoff between constant deleting and linear indexation and reverse
//its actually not like list but name means that is more easy to add or delete elements then just array
//<"=" == mov>
struct ListLikeMatrix_struct
{

    const VectorVectorInt * matrix_replace;

    VectorVectorListLikeMatrixNodeP matrix;

    VectorListLikeMatrixNodeP columns_headers;
    VectorListLikeMatrixNodeP lines_headers;
    //last indexes belong to headers
    VectorListLikeMatrixNodeP columns_finishers;
    VectorListLikeMatrixNodeP lines_finishers;
    VectorInt amount_of_ones_in_columns;

    int lines, columns;
    int absolute_lines, absolute_columns;
    int lines_start, columns_start;
    //when order is broken by addLine or like that, restoring to label becomes inaccessible
    bool maintain_deleting_and_restoring_order;

    //i can use push_backs on place because there is a lot of deleting but size wont be more than lines+columns at its little reallocs
    //also these are stacks but meh doesn't matter
    //first number is DeletingType
    VectorPairInt deleting_log;
    VectorInt deleting_labels;


    //int* (*atm)(struct ListLikeMatrix_struct * mat, int i, int j);
    //const int* (*catm)(const struct ListLikeMatrix_struct * mat, int i, int j);
    int (*getValue)(struct ListLikeMatrix_struct * mat, int i, int j);

    int (*getLines)(const struct ListLikeMatrix_struct * mat);
    int (*getColumns)(const struct ListLikeMatrix_struct * mat);

    void (*deleteLine)(struct ListLikeMatrix_struct * mat, ListLikeMatrixLineIterator line);
    void (*deleteColumn)(struct ListLikeMatrix_struct * mat, ListLikeMatrixColumnIterator column);
    //this is not external interface, user must restore only by labels
    void (*restoreLine)(struct ListLikeMatrix_struct * mat, ListLikeMatrixLineIterator line);
    void (*restoreColumn)(struct ListLikeMatrix_struct * mat, ListLikeMatrixColumnIterator column);

    void (*makeRestoringLabel)(struct ListLikeMatrix_struct * obj);
    void (*restore)(struct ListLikeMatrix_struct * obj);



    ListLikeMatrixLineIterator (*iterateOnLine)(ListLikeMatrixLineIterator it);
    ListLikeMatrixColumnIterator (*iterateOnColumn)(ListLikeMatrixColumnIterator it);

    ListLikeMatrixColumnIterator (*decrementOnColumn)(ListLikeMatrixColumnIterator it);

    ListLikeMatrixLineIterator (*linesBegin)(struct ListLikeMatrix_struct * mat);
    ListLikeMatrixColumnIterator (*columnsBegin)(struct ListLikeMatrix_struct * mat);

    ListLikeMatrixLineIterator (*linesEnd)(struct ListLikeMatrix_struct * mat);
    ListLikeMatrixColumnIterator (*columnsEnd)(struct ListLikeMatrix_struct * mat);

    ListLikeMatrixColumnIterator (*getColumnHeader)(ListLikeMatrixAnyIterator it);
    ListLikeMatrixColumnIterator (*getLineHeader)(ListLikeMatrixAnyIterator it);

    bool (*isFinish)(ListLikeMatrixAnyIterator it);

    int (*getAmountOfOnesInColumn)(struct ListLikeMatrix_struct * mat, ListLikeMatrixColumnIterator it);

    void (*addLine)(struct ListLikeMatrix_struct * mat, ListLikeMatrixColumnIterator iter);



};

int getLineOfIterator(ListLikeMatrixAnyIterator it);

typedef struct ListLikeMatrix_struct ListLikeMatrix;

ListLikeMatrix defaultListLikeMatrix(int lines, int columns, const VectorVectorInt * orig_matrix);
void destructListLikeMatrix(ListLikeMatrix * obj);

int getRealLineIndex(ListLikeMatrixColumnIterator it);
//after deleting, iterators need to be incremented since they still pointed to deleted line
void listLikeMatrixDeleteLine(ListLikeMatrix * mat, ListLikeMatrixLineIterator line);
void listLikeMatrixDeleteColumn(ListLikeMatrix * mat, ListLikeMatrixColumnIterator column);

static inline int listLikeMatrixGetLines(const ListLikeMatrix * mat)
{
    return mat->lines;
}
static inline int listLikeMatrixGetColumns(const ListLikeMatrix * mat)
{
    return mat->columns;
}


static inline ListLikeMatrixLineIterator listLikeMatrixIncrementLineIterator(ListLikeMatrixLineIterator it)
{
    ListLikeMatrixLineIterator res = it->next_on_line;
    while (!res->column_header->active)
        res = res->next_on_line;
    return it->next_on_line;
}
static inline ListLikeMatrixColumnIterator listLikeMatrixIncrementColumnIterator(ListLikeMatrixColumnIterator it)
{
    return it->next_on_column;
}

ListLikeMatrixLineIterator listLikeMatrixDecrementLineIterator(ListLikeMatrixLineIterator it);
static inline ListLikeMatrixColumnIterator listLikeMatrixDecrementColumnIterator(ListLikeMatrixColumnIterator it)
{
    return it->prev_on_column;
}


void listLikeMatrixRestoreLine(ListLikeMatrix * mat, ListLikeMatrixLineIterator it);
void listLikeMatrixRestoreColumn(ListLikeMatrix * mat, ListLikeMatrixColumnIterator it);

static inline ListLikeMatrixLineIterator listLikeMatrixGetLineBegin(ListLikeMatrix * mat)
{
    return *atVectorListLikeMatrixNodeP(&mat->lines_headers, mat->lines_start);
}
static inline ListLikeMatrixColumnIterator listLikeMatrixGetColumnBegin(ListLikeMatrix * mat)
{
    return *atVectorListLikeMatrixNodeP(&mat->columns_headers, mat->columns_start);
}

static inline ListLikeMatrixLineIterator listLikeMatrixGetLineEnd(ListLikeMatrix * mat)
{
    return *atVectorListLikeMatrixNodeP(&mat->lines_finishers, mat->absolute_lines);
}
static inline ListLikeMatrixColumnIterator listLikeMatrixGetColumnEnd(ListLikeMatrix * mat)
{
    return *atVectorListLikeMatrixNodeP(&mat->columns_finishers, mat->absolute_columns);
}

static inline ListLikeMatrixColumnIterator listLikeMatrixGetColumnHeader(ListLikeMatrixAnyIterator it)
{
    return it->column_header;
}
static inline bool listLikeMatrixIsFinish(ListLikeMatrixAnyIterator it)
{
    return it == NULL || it->line_finish == NULL || it->column_finish == NULL;
}

ListLikeMatrixColumnIterator getColumnStart(ListLikeMatrixAnyIterator it);

static inline ListLikeMatrixLineIterator listLikeMatrixGetLineHeader(ListLikeMatrixAnyIterator it)
{
    return it->line_header;
}

static inline void listLikeMatrixMakeRestoringLabel(ListLikeMatrix * mat)
{
    assert(mat->maintain_deleting_and_restoring_order);
    vectorIntPushBack(&mat->deleting_labels, mat->deleting_log.getSize(&mat->deleting_log));
}
void listLikeMatrixRestoreToLabel(ListLikeMatrix * mat);

void listLikeMatrixAssignFinishers(ListLikeMatrix * mat);

static inline int listLikeMatrixGetAmountOfOnesInColumn(ListLikeMatrix * mat, ListLikeMatrixColumnIterator column)
{
    return *atVectorInt(&mat->amount_of_ones_in_columns, column->column);
}

void listLikeMatrixAddLine(ListLikeMatrix * mat, ListLikeMatrixColumnIterator iter);

ListLikeMatrixLineIterator getLineOfRealIndex(ListLikeMatrix * mat, int index);

static inline const int * catListLikeMatrix(const ListLikeMatrix * mat, int i, int j)
{
    assert(i < mat->getLines(mat));
    assert(j < mat->getColumns(mat));
    assert(false);
    return NULL;
}

int listLikeMatrixGetValue(ListLikeMatrix * mat, int i, int j);


#endif //EXACT_COVER_LISTLIKEMATRIX_H
