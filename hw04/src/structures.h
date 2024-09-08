#include <time.h>

#ifndef STRUCTURES_H
#define STRUCTURES_H

struct transaction
{
    struct person *source;
    struct person *target;
    int amount;
    char *currency;
};

struct transactions
{
    struct transaction *transactions;
    int size;
    int capacity;
};

#endif
