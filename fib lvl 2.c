/*
* Algorithm is simple brute forsing: We try to fill graph for each variant of triplet's sum for each variant of 0s triplet and each sum's decompositon into triplet
* then we through circle through different variants branch. For every new triplet, we already know one of addictives and sum, so we need to try only one addictive
* on each step we checking if some digit is already in graph and can summ be splited this way in general
* so most of branches end rather quickly
* also as a result we get n of each combinations that correspond to cycle move, but we can sort result array and just ignore repetitions
*/


#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>

#define MAX_N 10

struct GraphFilling
{
    int inner[MAX_N];
    int outer[MAX_N];
    bool taken[2 * MAX_N];
    int n;
};

void initializeGraphFilling(struct  GraphFilling * obj)
{
    int i = 0;
    for (i = 0; i < 2 * MAX_N; ++i)
    {
        obj->taken[i] = false;
    }
    obj->n = 0;
}

void graphFillingSetInner(struct GraphFilling* obj, int index, int n, int size)
{
    obj->inner[index] = n;
    obj->taken[n] = true;
    obj->n = size;
}

void graphFillingSetOutter(struct GraphFilling* obj, int index, int n, int size)
{
    obj->outer[index] = n;
    obj->taken[n] = true;
    obj->n = size;
}

//that is needed because only small percent of entire start conditions variations will lead to some result
struct PrVector
{
    struct GraphFilling* arr;
    int located_mem;
    int index;
};

struct PrVector defaultPrVector()
{
    struct PrVector vec;
    vec.arr = NULL;
    vec.located_mem = 0;
    vec.index = 0;
    return vec;
}

void prVectorAddElement(struct PrVector* vec, int index, struct GraphFilling *element)
{
    struct GraphFilling* new_arr = NULL;
    if (vec->located_mem <= index)
    {
        new_arr = (struct GraphFilling*)calloc(2 * vec->located_mem + 1, sizeof(struct GraphFilling));
        assert(new_arr != NULL);
        memcpy((void*)new_arr, (void*)vec->arr, vec->located_mem * sizeof(struct GraphFilling));
        memcpy((void*)&new_arr[index], (void*)element, sizeof(struct GraphFilling));
        free(vec->arr);
        vec->arr = new_arr;
        vec->located_mem = 2 * vec->located_mem + 1;
    }
    else
        memcpy((void*)&vec->arr[index], (void*)element, sizeof(struct GraphFilling));
}

void prVectorPushBack(struct PrVector* vec, struct GraphFilling element)
{
    prVectorAddElement(vec, vec->index, &element);
    ++vec->index;
}

bool isInRange(int x, int a, int b)
{
    return x >= a && x <= b;
}
bool areDifferent(int a, int b, int c)
{
    return a != b && a != c && b != c;
}
bool isValidSumSplit(int a, int b, int c, int s, int n)
{
    return a + b + c == s && areDifferent(a, b, c) && isInRange(a, 1, 2*n) && isInRange(b, 1, 2*n) && isInRange(c, 1, 2*n);
}

void tryToFillExactStartCond(int s, int n, int depth_index, const struct GraphFilling* cond, struct PrVector * res);

void makeSumAttempt(int first_sum, int second_sum, int third_sum, int n, int depth_index, const struct GraphFilling * cond, struct PrVector * res)
{
    int i = 0;
    bool success = false;
    struct GraphFilling attempt_graph;
    memcpy((void*)&attempt_graph, (void*)cond, sizeof(struct GraphFilling));

    graphFillingSetOutter(&attempt_graph, depth_index, first_sum, n);
    graphFillingSetInner(&attempt_graph, depth_index + 1, third_sum, n);

    struct PrVector vec = defaultPrVector();
    tryToFillExactStartCond(first_sum + second_sum + third_sum, n, depth_index + 1, &attempt_graph, &vec);

    for (i = 0; i < vec.index; ++i)
        prVectorPushBack(res, vec.arr[i]);

}

void tryLastTriplet(int n, int s, const struct GraphFilling * cond, struct PrVector * res)
{
    int second_sum = cond->inner[n - 1];
    int third_sum = cond->inner[0];
    int first_sum = s - second_sum - third_sum;
    if (!cond->taken[first_sum] && isValidSumSplit(first_sum, second_sum, third_sum, s, n))
    {
        struct GraphFilling fil = *cond;
        graphFillingSetOutter(&fil, n - 1, first_sum, n);
        prVectorPushBack(res, fil);
        return;
    }
}
void tryToFillExactStartCond(int s, int n, int depth_index, const struct GraphFilling* cond, struct PrVector * res)
{
    assert(cond != NULL && res != NULL);
    int i = 0, j = 0;
    int first_sum = 0, second_sum, third_sum = 0;

    if (depth_index >= n)
        return;
    if (depth_index == n - 1)
    {
        tryLastTriplet(n, s, cond, res);
    }
    for (i = 1; i <= 2*n; ++i)
    {
        if (cond->taken[i])
            continue;

        first_sum = i;
        second_sum = cond->inner[depth_index];
        third_sum = s - first_sum - second_sum;

        if (!isValidSumSplit(first_sum, second_sum, third_sum, s, n))
            continue;

        if (cond->taken[third_sum])
            continue;

        makeSumAttempt(first_sum, second_sum, third_sum, n, depth_index, cond, res);
    }
}


void tryToFillExactSum(int s, int n, struct PrVector * result)
{
    int i = 0, j = 0;
    //PrVector result = defaultPrVector();

    for (i = 1; i <= 2 * n; ++i)
    {
        for (j = 1; j <= 2 * n && s - i - j > 0; ++j)
        {

            if (!isValidSumSplit(i, j, s - i - j, s, n))
                continue;
            struct GraphFilling cond;
            initializeGraphFilling(&cond);

            graphFillingSetOutter(&cond, 0, i, n);
            graphFillingSetInner(&cond, 0, j, n);
            graphFillingSetInner(&cond, 1, s - i - j, n);

            tryToFillExactStartCond(s, n, 1, &cond, result);
        }
    }
}

struct PrVector findAllCombinations(int n)
{
    int i = 0, j = 0;
    // Node* sums = getAllSumsCombinations(n);
    struct PrVector res = defaultPrVector();
    for (i = 6; i <= 6 * n - 3; ++i)
    {
        tryToFillExactSum(i, n, &res);
    }
    return res;
}

void printfTriplet(int a, int b, int c)
{
    printf("%d, %d, %d;   ", a, b, c);
}

int findMinIndex(int* circle, int n)
{
    int min_d = 2 * n;
    int i = 0;
    int res_index = 0;
    for (i = 0; i < n; ++i)
    {
        if (circle[i] < min_d)
        {
            min_d = circle[i];
            res_index = i;
        }
    }
    return res_index;
}

void outCircle(struct GraphFilling * graph, int n)
{
    int start = findMinIndex(graph->outer, n);
    int i = 0;
    for (i = 0; i < n; ++i)
    {
        printfTriplet(graph->outer[(i + start) % n],
                      graph->inner[(i + start) % n],
                      graph->inner[(i + start + 1) % n]);
    }
    printf("\n");
}

int graphFillingLess(const void* ap, const void* bp)
{
    struct GraphFilling* a = (struct GraphFilling*)ap;
    struct GraphFilling* b = (struct GraphFilling*)bp;

    int min_a = findMinIndex(a->outer, a->n);
    int min_b = findMinIndex(b->outer, b->n);

    int i = 0;

    for (i = 0; i < a->n; ++i)
    {
        if (a->outer[(i + min_a) % a->n] < b->outer[(i + min_b) % a->n])
            return -1;
        if (a->outer[(i + min_a) % a->n] > b->outer[(i + min_b) % a->n])
            return 1;

        if (a->inner[(i + min_a) % a->n] < b->inner[(i + min_b) % a->n])
            return -1;
        if (a->inner[(i + min_a) % a->n] > b->inner[(i + min_b) % a->n])
            return 1;

        if (a->inner[(i + min_a + 1) % a->n] < b->inner[(i + min_b + 1) % a->n])
            return -1;
        if (a->inner[(i + min_a + 1) % a->n] > b->inner[(i + min_b + 1) % a->n])
            return 1;
    }

    return 0;

}

int main()
{
    int n = 0;
    int i = 0;

    if (!scanf("%d", &n))
        return 0;
    if (n > MAX_N)
        return 0;

    struct PrVector combs = findAllCombinations(n);
    qsort((void*)combs.arr, combs.index, sizeof(struct GraphFilling), graphFillingLess);
    for (i = 0; i < combs.index; i += n)
    {
        outCircle(&combs.arr[i], n);
    }
}