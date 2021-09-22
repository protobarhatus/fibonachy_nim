#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>

static long long int binomialCoefficient(int n, int k)
{
    const int N = 30;
    static long long int binomial_coefficients[N + 1][N + 1];
    int i = 0, j = 0;
    static bool was_builded = false;

    assert(n <= 30);
    assert(n >= k);
    if (was_builded)
    {
        return binomial_coefficients[n][k];
    }
    was_builded = true;

    binomial_coefficients[0][0] = 1;
    binomial_coefficients[1][0] = 1;
    binomial_coefficients[1][1] = 1;
    for (i = 2; i <= N; ++i)
    {
        binomial_coefficients[i][0] = 1;
        binomial_coefficients[i][i] = 1;
        for (j = 1; j < i; ++j)
            binomial_coefficients[i][j] = binomial_coefficients[i - 1][j - 1] + binomial_coefficients[i - 1][j];
    }

    return binomial_coefficients[n][k];
}
#define MAX_N 10

struct Node
{
    //thats array
    Node* next;
    
    int num;
};

static int min(int a, int b)
{
    return a < b ? a : b;
}

/*bool callocThreeDeepNode(Node* external, int n, int s)
{
    int i = 0, j = 0;
    int max_f = min(2 * n, s - 3);
    int c;
    bool has_cont_in = false;
    bool has_cont_ext = false;
    external->next = (Node*)calloc(2 * n + 1, sizeof(Node));
    for (i = 1; i <= max_f; ++i)
    {

        external->next[i].next = (Node*)calloc(2 * n + 1, sizeof(Node));
        has_cont_in = false;
        for (j = 1; j <= max_f; ++j)
        {
            if (j == i)
            {
                external->next[i].next[j].num = -1;
                continue;
            }
            c = s - i - j;
            if (c == i || c == j || c <= 0 || c > 2 * n)
                external->next[i].next[j].num = -1;
            else
            {
                external->next[i].next[j].num = c;
                has_cont_in = true;
            }
        }
        if (!has_cont_in)
        {
            free(external->next[i].next);
            external->next[i].next = NULL;
            external->next[i].num = -1;
        }
        else
        {
            has_cont_ext = true;
            external->next[i].num = i;
        }
    }
    if (has_cont_ext)
        return true;
    free(external->next);
    external->next = NULL;
    external->num = -1;
    return false;

    
}

Node* getAllSumsCombinations(int n)
{
    int i = 0;
    Node* sums = (Node*)calloc(6 * n - 3 + 1, sizeof(Node));
    for (i = 0; i < 6; ++i)
    {
        sums[i].next = NULL;
        sums[i].num = -1;
    }
    for (i = 6; i <= 6 * n - 3; ++i)
    {
        bool res = callocThreeDeepNode(&sums[i], n, i);
        if (!res)
        {
            sums[i].next = NULL;
            sums[i].num = -1;
        }
    }

    return sums;

}*/

struct GraphFilling
{
    int inner[MAX_N];
    int outer[MAX_N];
    bool taken[2 * MAX_N] = { false };
    int n;
};
void graphFillingSetInner(GraphFilling* obj, int index, int n, int size)
{
    obj->inner[index] = n;
    obj->taken[n] = true;
    obj->n = size;
}
void graphFillingSetOutter(GraphFilling* obj, int index, int n, int size)
{
    obj->outer[index] = n;
    obj->taken[n] = true;
    obj->n = size;
}

//that is needed because only small percent of entire start conditions variations will lead to some result
struct PrVector
{
    GraphFilling* arr;
    int located_mem;
    int index = 0;
};

PrVector defaultPrVector()
{
    PrVector vec;
    vec.arr = NULL;
    vec.located_mem = 0;
    vec.index = 0;
    return vec;
}

void prVectorAddElement(PrVector* vec, int index, GraphFilling element)
{
    GraphFilling* new_arr = NULL;
    if (vec->located_mem <= index)
    {
        new_arr = (GraphFilling*)calloc(2 * vec->located_mem + 1, sizeof(GraphFilling));
        memcpy((void*)new_arr, (void*)vec->arr, vec->located_mem * sizeof(GraphFilling));
        memcpy((void*)&new_arr[index], (void*)&element, sizeof(GraphFilling));
        free(vec->arr);
        vec->arr = new_arr;
        vec->located_mem = 2 * vec->located_mem + 1;
    }
    else
        memcpy((void*)&vec->arr[index], (void*)&element, sizeof(GraphFilling));
}
void prVectorPushBack(PrVector* vec, GraphFilling element)
{
    prVectorAddElement(vec, vec->index, element);
    ++vec->index;
}

bool isValidSumSplit(int a, int b, int c, int s, int n)
{
    return a + b + c == s && a != b && a != c && b != c && a > 0 && b > 0 && c > 0 && a <= 2 * n && b <= 2 * n && c <= 2 * n;
}
bool tryToFillExactStartCond(int s, int n, int depth_index, const GraphFilling* cond, PrVector * res)
{
    assert(cond != nullptr && res != nullptr);
    int i = 0, j = 0;
    int first_sum = 0, second_sum, third_sum = 0;
    bool succes = false;
    bool found_someting = false;

    if (depth_index >= n)
        return true;
    if (depth_index == n - 1)
    {
        second_sum = cond->inner[depth_index];
        third_sum = cond->inner[0];
        first_sum = s - second_sum - third_sum;
        if (!cond->taken[first_sum] && isValidSumSplit(first_sum, second_sum, third_sum, s, n))
        {
            GraphFilling fil = *cond;;
            graphFillingSetOutter(&fil, depth_index, first_sum, n);
            prVectorPushBack(res, fil);
            return true;
        }
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

            GraphFilling attempt_graph;
            memcpy((void*)&attempt_graph, (void*)cond, sizeof(GraphFilling));
            graphFillingSetOutter(&attempt_graph, depth_index, first_sum, n);
            graphFillingSetInner(&attempt_graph, depth_index + 1, third_sum, n);
            PrVector vec = defaultPrVector();
            succes = tryToFillExactStartCond(s, n, depth_index + 1, &attempt_graph, &vec);
            if (succes)
            {
                //free(cond);
               // memcpy((void*)cond, (void*)&attempt_graph, sizeof(GraphFilling));
                for (j = 0; j < vec.index; ++j)
                    prVectorPushBack(res, vec.arr[j]);
                found_someting = true;
            }


    }
    return found_someting;
}


PrVector tryToFillExactSum(int s, int n)
{
    int i = 0, j = 0;
    bool succes = false;
    PrVector result = defaultPrVector();

    for (i = 1; i <= 2 * n; ++i)
    {
   //     if (sum_split[i].num <= 0)
    //    {
     //       continue;
    //    }
        for (j = 1; j <= 2 * n && s - i - j > 0; ++j)
        {
         //   if (sum_split[j].num <= 0)
       //     {
      //          continue;
      //      }

            if (!isValidSumSplit(i, j, s - i - j, s, n))
                continue;
            GraphFilling cond;
            graphFillingSetOutter(&cond, 0, i, n);
            graphFillingSetInner(&cond, 0, j, n);
            graphFillingSetInner(&cond, 1, s - i - j, n);
            
            succes = tryToFillExactStartCond(s, n, 1, &cond, &result);
            if (succes)
            {
               // prVectorPushBack(&result, cond);
            }
        }
    }
    return result;
}

PrVector findAllCombinations(int n)
{
    int i = 0, j = 0;
   // Node* sums = getAllSumsCombinations(n);
    PrVector res = defaultPrVector();
    for (i = 6; i <= 6 * n - 3; ++i)
    {
        PrVector combs = tryToFillExactSum(i, n);
        for (j = 0; j < combs.index; ++j)
            prVectorPushBack(&res, combs.arr[j]);
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

void outCircle(GraphFilling * graph, int n)
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

int ghaphFillingLess(const void* ap, const void* bp)
{
    GraphFilling* a = (GraphFilling*)ap;
    GraphFilling* b = (GraphFilling*)bp;

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

    PrVector combs = findAllCombinations(n);
    qsort((void*)combs.arr, combs.index, sizeof(GraphFilling), ghaphFillingLess);
    for (i = 0; i < combs.index; i += n)
    {
        outCircle(&combs.arr[i], n);
    }
}

/*4, 2, 3;   5, 3, 1;   6, 1, 2;
4, 3, 2;   6, 2, 1;   5, 1, 3;
2, 3, 5;   4, 5, 1;   6, 1, 3;
2, 5, 3;   6, 3, 1;   4, 1, 5;
1, 4, 6;   3, 6, 2;   5, 2, 4;
1, 6, 4;   5, 4, 2;   3, 2, 6;
1, 5, 6;   2, 6, 4;   3, 4, 5;
1, 6, 5;   3, 5, 4;   2, 4, 6;
1, 6, 5;   3, 5, 4;   2, 4, 6;
4, 3, 1;   5, 1, 2;   6, 2, 3;
4, 2, 1;   6, 1, 3;   5, 3, 2;
2, 5, 1;   4, 1, 3;   6, 3, 5;
2, 3, 1;   6, 1, 5;   4, 5, 3;
1, 6, 2;   3, 2, 4;   5, 4, 6;
1, 4, 2;   5, 2, 6;   3, 6, 4;
1, 6, 4;   2, 4, 5;   3, 5, 6;
1, 5, 4;   3, 4, 6;   2, 6, 5;
1, 5, 4;   3, 4, 6;   2, 6, 5;*/