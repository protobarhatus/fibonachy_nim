#include "user_interactions.h"
#include <string.h>
#include <stdio.h>

ExecutionLogMode chooseLogMode(int argc, char * argv[])
{
    if (argc == 1)
        return EXECUTION_LOG_MODE_NONE;
    if (strcmp(argv[1], "heartbeat") == 0)
        return EXECUTION_LOG_MODE_HEARTBEAT;
    if (strcmp(argv[1], "countdown") == 0)
        return EXECUTION_LOG_MODE_COUNTDOWN;

    return EXECUTION_LOG_MODE_NONE;
}

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