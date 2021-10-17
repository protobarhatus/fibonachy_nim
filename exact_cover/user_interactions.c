#include "user_interactions.h"
#include <string.h>
#include <stdio.h>


static void clearBuff()
{
    char c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

int getValue(FILE * in)
{
    int num;
    while (fscanf(in,"%d", &num) == 0)
    {
        printf("Incorrect input\n");
        clearBuff();

    }
    return num;
}