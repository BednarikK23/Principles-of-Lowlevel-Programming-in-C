// #desc: Testy nanečisto
#define CUT
#include "libs/cut.h"

#include "libs/mainwrap.h"

#include <stdlib.h>


// #desc: encode(): Nuly
TEST(nanecisto_encode_zeroes)
{
    /* Array of input bytes. 4 bytes with value 0.
     * NOTE: numbers after prefix '0x' are represent in base 16.
     * */
    const unsigned char input[] = {
        0x00, 0x00, 0x00, 0x00,
    };
    // Array of expected bytes on standard output.
    const unsigned char output[] = {
        0x00, 0x00, 0x00, 0x00, 0x00,
    };

    // This statement will replace standard input of the program for input array.
    INPUT_BYTES(input);

    /* This statement will check if the expression is true.
     * Calling `app_main(ARGS)` is equivalent to running your
     * program with arguments ARGS.
     * The return value of `app_main` is value returned from your main() function in main.c
     * */
    CHECK(app_main("-e") == EXIT_SUCCESS);
    // This statement will check whether the standard error output is empty.
    CHECK_FILE(stderr, "");
    // This statement will check whether the standard output equals to output array.
    CHECK_BINARY_FILE(stdout, output);
}

// #desc: decode(): nuly
TEST(nanecisto_decode_zeroes)
{
    const unsigned char input[] = { 0x00, 0x00, 0x00, 0x00, 0x00 };
    const unsigned char output[] = { 0x00, 0x00, 0x00, 0x00 };

    INPUT_BYTES(input);
    CHECK(app_main("-d") == EXIT_SUCCESS);
    CHECK_FILE(stderr, "");
    CHECK_BINARY_FILE(stdout, output);
}

// #desc: encode(): príklad zo zadania
TEST(nanecisto_encode_assignment_example)
{
    const unsigned char input[] = { 0x00, 0x01, 0x02, 0x03 };
    const unsigned char output[] = { 0x20, 0x80, 0x04, 0x08, 0x06 };
    INPUT_BYTES(input);
    CHECK(app_main("-e") == EXIT_SUCCESS);
    CHECK_FILE(stderr, "");
    CHECK_BINARY_FILE(stdout, output);
}

// #desc: decode(): príklad zo zadania
TEST(nanecisto_decode_assignment_example)
{
    const unsigned char input[] = { 0x20, 0x80, 0x04, 0x08, 0x06 };
    const unsigned char output[] = { 0x00, 0x01, 0x02, 0x03 };
    INPUT_BYTES(input);
    CHECK(app_main("-d") == EXIT_SUCCESS);
    CHECK_FILE(stderr, "");
    CHECK_BINARY_FILE(stdout, output);
}
