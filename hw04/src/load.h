#include "persons.h"

#include <stdio.h>

#ifndef LOAD_H_
#define LOAD_H_

void load_currency_table(struct currency_table *table, FILE *input);
void load_persons(struct persons *persons, FILE *input);
void load_payments(struct persons *persons, struct currency_table *table, FILE *input);

#endif
