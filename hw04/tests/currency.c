#include "libs/cut.h"
#include "libs/utils.h"
#include "libs/mainwrap.h"

#include <string.h>

#include "../src/constants.h"
#include "../src/currency.h"
#include "../src/decimals.h"
#include "../src/errors.h"

TEST(currency_init) {
    struct currency_table table;
    memset(&table, 0, sizeof(table));
    init_currency_table(&table);

    CHECK(table.size == 0);
    CHECK(table.capacity == 16);

    object_destroy(&table);
}

TEST(currency_add_find) {
    enum error_codes error_code;
    enum error_codes expected_code = SUCCESS;
    struct currency_table table;
    memset(&table, 0, sizeof(table));
    init_currency_table(&table);

    stack_top_frame();
    if ((error_code = read_error_point())) {
        object_destroy(&table);
        if (error_code != expected_code) {
            DEBUG_MSG("Expected %d error code; %d given", expected_code, error_code);
            ASSERT(0);
        }
        return;
    }

    SUBTEST(add only main currency) {
        add_currency(&table, "CZK", 0);

        ASSERT(10 == convert_currency(&table, "CZK", 10));
    }
    SUBTEST(convert with empty table) {
        expected_code = CURRENCY_NOT_FOUND;
        convert_currency(&table, 10, "CZK");
    }
    SUBTEST(simple convert ) {
        add_currency(&table, "A", load_decimal("2", RATING_DECIMALS));
        ASSERT(20 == convert_currency(&table, 10, "A"));
    }
    SUBTEST(duplicit currency) {
        expected_code = CURRENCY_ALREADY_PRESENT;
        add_currency(&table, "A", 2);
        add_currency(&table, "A", 2);
    }
    SUBTEST(add a lot of currencies) {
        char name[5];
        int rating = load_decimal("2", RATING_DECIMALS);
        for (int i = 0; i < 40; ++i) {
            sprintf(name, "c%.2ic", i);
            add_currency(&table, name, rating);
        }
        CHECK(table.size == 40);
        CHECK(table.capacity == 64);

        CHECK(20 == convert_currency(&table, 10, "c06c"));
        CHECK(20 == convert_currency(&table, 10, "c39c"));
    }

    exit_success();
}
