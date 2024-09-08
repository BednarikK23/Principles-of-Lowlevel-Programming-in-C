#include "currency.h"
#include "errors.h"
#include "load.h"
#include "persons.h"
#include "structures.h"

#include <string.h>

struct person *find_extreme(struct persons *persons, int sign)
{
    if (!persons->size)
        return NULL;
    struct person *extreme = &persons->persons[0];
    for (int i = 1; i < persons->size; ++i) {
        if (persons->persons[i].amount * sign > extreme->amount)
            extreme = &persons->persons[i];
    }
    return extreme;
}

void settle_debt(struct persons *persons)
{
    while (1) {
        struct person *debtor = find_extreme(persons, -1);
        struct person *creditor = find_extreme(persons, 1);

        int amount = -debtor->amount;
        if (amount > creditor->amount)
            amount = creditor->amount;

        debtor->amount += amount;
        creditor->amount -= amount;
        printf("%s -> %s %d\n", debtor->id, creditor->id, amount);
    }
}

int main(int argc, char **argv)
{
    stack_top_frame();
    enum error_codes error_code;
    struct currency_table currency_table;
    struct persons persons;
    FILE *person_file = NULL;
    FILE *currency_file = NULL;
    FILE *payment_file = NULL;
    memset(&currency_table, 0, sizeof(currency_table));
    memset(&persons, 0, sizeof(persons));

    if ((error_code = read_error_point())) {
        object_destroy(&currency_table);
        object_destroy(&persons);
        person_file &&fclose(person_file);
        currency_file &&fclose(currency_file);
        payment_file &&fclose(payment_file);

        if (error_code != SUCCESS)
            print_error_message(error_code);

        return return_code(error_code);
    }
    OP(argc == 3, INVALID_ARGUMENTS);

    init_currency_table(&currency_table);
    init_persons(&persons);

    person_file = fopen(argv[1], "r");
    load_persons(&persons, person_file);

    currency_file = fopen(argv[2], "r");
    load_currency_table(&currency_table, currency_file);

    payment_file = fopen(argv[3], "r");
    load_payments(&persons, &currency_table, payment_file);

    settle_debt(&persons);

    exit_success();
}
