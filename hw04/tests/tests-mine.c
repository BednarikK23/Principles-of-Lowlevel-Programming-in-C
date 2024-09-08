#include "libs/cut.h"
#include "libs/utils.h"
#include "libs/mainwrap.h"

/*
 * You can use this file for your tests.
 * All tests will compile together into one binary.
 * You can also delete your colleagues test files,
 * if you decide not to use them.
 */

TEST(my_test)
{
    int rv = 0;
    CHECK(app_main_args("--bonus" /* args */) == rv);
    /* use app_main() if you don't want to give any args */

    const char *correct_output = "Output\n";

    CHECK_NOT_EMPTY(stdout);
    ASSERT_FILE(stdout, correct_output);
    CHECK_IS_EMPTY(stderr);
}
