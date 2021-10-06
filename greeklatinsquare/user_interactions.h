#ifndef GREEKLATINGSQ_USER_INTERACTIONS_H
#define GREEKLATINGSQ_USER_INTERACTIONS_H
#include <stdio.h>
enum ExecutionLogMode_enum
{
    EXECUTION_LOG_MODE_NONE,
    EXECUTION_LOG_MODE_HEARTBEAT,
    EXECUTION_LOG_MODE_COUNTDOWN
};
typedef enum ExecutionLogMode_enum ExecutionLogMode;

ExecutionLogMode chooseLogMode(int argc, char * argv[]);

int getValue(FILE * input);

#endif //GREEKLATINGSQ_USER_INTERACTIONS_H
