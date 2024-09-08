#include "libs/cut.h"
#include "libs/utils.h"
#include "libs/mainwrap.h"

#define PERSONS_SIMPLEST "tests/data/nanecisto/persons_simplest.txt" 
#define CURRENCIES_SIMPLEST "tests/data/nanecisto/currencies_simplest.txt"
#define PAYMENTS_SIMPLEST "tests/data/nanecisto/payments_simplest.txt"

#define PAYMENTS_NO_SETTLE "tests/data/nanecisto/payments_noSettle.txt"

#define PERSONS_DUPE_CURR "tests/data/nanecisto/persons_dupeCurrency.txt" 
#define CURRENCIES_DUPE_CURR "tests/data/nanecisto/currencies_dupeCurrency.txt"

#define PERSONS_DUPE_PERSON "tests/data/nanecisto/persons_dupePerson.txt" 
#define CURRENCIES_DUPE_PERSON "tests/data/nanecisto/currencies_dupePerson.txt"
#define PAYMENTS_DUPE "tests/data/nanecisto/payments_dupe.txt"

/*
 * Tests expect the compiled test file to be in directory source/build.
 * Tests are working with files in source/tests/data/nanecisto directory only.
 *
 * Your colleague Ládínek has written some extra testing files.
 * They are in source/tests/data/extra directory. Feel free to use them.
 * They will not be used by tests-nanecisto.
 */

TEST(simplest_transaction)
{
    CHECK(app_main_args(PERSONS_SIMPLEST, 
                        CURRENCIES_SIMPLEST, 
                        PAYMENTS_SIMPLEST) == 0);

    const char *correct_output = "Bob (BB) -> Alice (AA): 1 CZK\n";

    ASSERT_FILE(stdout, correct_output);
    CHECK_IS_EMPTY(stderr);
}

TEST(no_settle_needed)
{
    CHECK(app_main_args(PERSONS_SIMPLEST, 
                        CURRENCIES_SIMPLEST, 
                        PAYMENTS_NO_SETTLE) == 0);
    CHECK_IS_EMPTY(stdout);
    CHECK_IS_EMPTY(stderr);
}

TEST(no_arguments)
{
    CHECK(app_main() != 0);
    CHECK_NOT_EMPTY(stderr);
}

TEST(dupe_currency)
{
    CHECK(app_main_args(PERSONS_DUPE_CURR,
                        CURRENCIES_DUPE_CURR,
                        PAYMENTS_DUPE) != 0);
    CHECK_NOT_EMPTY(stderr);
}

TEST(dupe_person)
{
    CHECK(app_main_args(PERSONS_DUPE_PERSON, 
                        CURRENCIES_DUPE_PERSON,
                        PAYMENTS_DUPE) != 0);
    CHECK_NOT_EMPTY(stderr);
}
