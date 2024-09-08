#include "currency.h"

#include "constants.h"
#include "decimals.h"
#include "errors.h"
#include "utils.h"

#include <stdlib.h>
#include <string.h>

static void swap(struct currency *lhs, struct currency *rhs)
{
    char *name = lhs->name;
    int rating = lhs->rating;

    lhs->name = rhs->name;
    lhs->rating = rhs->rating;

    rhs->name = name;
    rhs->rating = rating;
}

static void ensort(struct currency_table *table)
{
    struct currency *current = table->currencies + table->size - 1;
    for (; current != table->currencies; --current) {
        struct currency *previous = current - 1;
        if (strcmp(previous->name, current->name) <= 0)
            break;
        swap(previous, current);
    }
}

static int currency_cmp(const void *_name, const void *_currency)
{
    const char *name = (const char *) _name;
    const struct currency *currency = (const struct currency *) _currency;
    return strcmp(name, currency->name);
}

struct currency *find_currency(struct currency_table *table, const char *name)
{
    return (struct currency *) bsearch(name, table->currencies, table->size, sizeof(struct currency), currency_cmp);
}

static void destroy_currency_table(void *t)
{
    struct currency_table *table = (struct currency_table *) t;

    for (int i = 0; i != table->size; ++i) {
        free(table->currencies[i].name);
    }
    free(table->currencies);
}

void init_currency_table(struct currency_table *table)
{
    object_avoid_duplicit_initialization(table);
    table->capacity = 16;
    table->main_currency = 0;
    table->size = 0;
    OP(table->currencies = (struct currency *) malloc(sizeof(struct currency) * table->capacity), ALLOCATION_FAILED);
    object_set_destructor(table, destroy_currency_table);
}

void add_currency(struct currency_table *table, const char *name, int rating)
{
    if (!rating && table->main_currency)
        error_happened(CURRENCY_DUPLICATE_MAIN);

    OP(!find_currency(table, name), CURRENCY_ALREADY_PRESENT);

    if (table->size == table->capacity) {
        table->capacity *= 2;
        struct currency *currencies;
        OP(currencies = (struct currency *) realloc(table->currencies, table->capacity * sizeof(struct currency)), ALLOCATION_FAILED);
        table->currencies = currencies;
    }

    table->currencies[table->size].name = copy_string(name);
    table->currencies[table->size].rating = rating;

    if (!rating)
        table->main_currency = table->currencies[table->size].name;
    ++table->size;
}

int convert_currency(struct currency_table *table, int amount, const char *currency)
{
    struct currency *found;
    OP(found = find_currency(table, currency), CURRENCY_NOT_FOUND);
    if (!found->rating)
        return amount;
    return amount * found->rating / decimals_to_base(RATING_DECIMALS);
}
