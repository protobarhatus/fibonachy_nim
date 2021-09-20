#include <stdlib.h>
#include <stdio.h>

#define bool int
#define true 1
#define false 0
#define nullptr NULL
#define FIB_AMOUNT 47

int* buildFibonacci()
{
	static bool was_builded = false;
	static int* fib = nullptr;
	int index = 2;
	if (was_builded)
	{
		return fib;
	}
	{
		//heap because i dont know how to make it on stack so that it wont deletes after return from function
		fib = (int *)calloc(FIB_AMOUNT, sizeof(int));
	}

	fib[0] = 1;
	fib[1] = 1;

	for (index = 2; index < FIB_AMOUNT; ++index)
		fib[index] = fib[index - 1] + fib[index - 2];

	return fib;
}

int findNextIndex(int prim, int* fib, int num)
{
	int next = prim - 2;
	while (next > 0 && fib[next] > num)
		--next;
	return next;
}
int firstFibonacciInItsBaseSystem(int num)
{
	int next_index, i, result;
	int* fib = nullptr;
	int last_sub_index = FIB_AMOUNT;

	fib = buildFibonacci();

	next_index = findNextIndex(FIB_AMOUNT, fib, num);

	while (num > 0)
	{
		last_sub_index = next_index;
		num -= fib[next_index];
		next_index = findNextIndex(next_index, fib, num);
	}

	return fib[last_sub_index];
}

int next_turn(int total, int possible)
{
	int to_take;
	if (total <= 0) //  there is no assert because its meaningfull to take zero sticks if its 0 of them or less and thats external error that external program must deal with
		return 0;
	if (total == 1)
		return 1;
	if (possible >= total)
		return total;

	to_take = firstFibonacciInItsBaseSystem(total);
	if (to_take > possible)
		return 1;
	return to_take;

}


bool isFib(int num, int* fib)
{
	int i = 0;
	for (i = 0; i < FIB_AMOUNT && fib[i] <= num; ++i)
	{
		if (fib[i] == num)
			return true;
	}
	return false;
}

int max(int a, int b)
{
	return a > b ? a : b;
}
bool doRandomGame(int start, int poss)
{
	int total = start, its_step = 0, possible = poss, rand_step = 1;
	while (total > 0)
	{
		its_step = next_turn(total, poss);
		total -= start;
		possible = its_step * 2;
		if (total == 0)
			return true;

		rand_step = rand() % (max(possible, total) - 1) + 1;
		total -= rand_step;
		possible = rand_step * 2;
	}
	return false;
}

int main()
{
	int* fib = buildFibonacci();
	int i = 0, start = 0, poss = 0, wins = 0;
	for (i = 0; i < 1000000; ++i)
	{
		do
		{
			start = rand() % 45 + 1;
		} while (isFib(start, fib));
		poss = rand() % (start - 1);

		wins += doRandomGame(start, poss);
		if (i % 1000 == 0)
			printf("%d%%\n", int((i / 1000000.0) * 100));
	}
	printf("-----------------");
	printf("Winrate: %g%%\n", (double(wins) / 1000000) * 100);
	return 0;
}