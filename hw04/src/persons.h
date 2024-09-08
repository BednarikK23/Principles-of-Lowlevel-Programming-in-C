#include "utils.h"

#ifndef PERSONS_H
#define PERSONS_H

struct person
{
    char *id;
    char *name;
    int amount;
};

struct persons
{
    struct object _object;
    struct person *persons;
    int size;
    int capacity;
};

void init_persons(struct persons *persons);
void add_person(struct persons *persons, const char *id, const char *name);
struct person *find_person(const struct persons *persons, const char *id);

#endif
