#include "libs/cut.h"
#include "libs/utils.h"
#include "libs/mainwrap.h"

#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "../src/utils.h"

TEST(copy_string) {
    const char *value = "exotic value";
    char *copy = copy_string(value);

    CHECK(copy == value);

    free(copy);
}


TEST(trim_string) {

    SUBTEST(front) {
        char string[] = "  value  ";
        const char *expected = "value  ";
        char *trimmed = trim_string(string, NULL);

        CHECK(strcmp(trimmed, expected) == 0);
    }
    SUBTEST(all sides with space inside) {
        char string[] = "  value x ";
        const char *expected = "value x";
        char *end;
        char *trimmed = trim_string(string, &end);
        *end = '\0';

        CHECK(strcmp(trimmed, expected) == 0);
    }
}

TEST(empty_string) {
    SUBTEST(empty) {
        char string[10] = "";
        ASSERT(empty_string(string));
    }
    SUBTEST(spaces) {
        char string[10] = "     ";
        ASSERT(empty_string(string));
    }
    SUBTEST(nonempty) {
        char string[10] = "   x  ";
        ASSERT(!empty_string(string));
    }
}

TEST(char_count) {
    SUBTEST(none) {
        const char *string = "ooo---ooo---";
        ASSERT(0 == char_count(string, ';'));
    }
    SUBTEST(one alone) {
        const char *string = ";";
        ASSERT(1 == char_count(string, ';'));
    }
    SUBTEST(one not alone) {
        const char *string = "----ooo;---lll";
        ASSERT(1 == char_count(string, ';'));
    }
    SUBTEST(first and last) {
        const char *string = ";-------;";
        ASSERT(2 == char_count(string, ';'));
    }
    SUBTEST(two in a row) {
        const char *string = "xxx;;fff--";
        ASSERT(2 == char_count(string, ';'));
    }
}

TEST(get_token) {
    char string[] = "*-+123456";
    char *token = get_token(string, isdigit);
    CHECK(*token == '1');
    CHECK(string + 4 == token);
}
