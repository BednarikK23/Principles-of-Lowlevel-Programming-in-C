#define CUT

#include "libs/cut.h"
#include "libs/mainwrap.h"

#include <stdlib.h>

TEST(P1_win) {
    INPUT_STRING(
        "3d 3h\n"
        "Ac Ts\n"
        "Qd 8s 2c 4c Kh\n"
    );

    app_main();
    ASSERT_FILE(stdout, "Player 1\n");
}

TEST(Draw) {
    INPUT_STRING(
        "4s 8d\n"
        "5c 8h\n"
        "7c 7h Ac Kd 2d\n"
    );

    app_main();
    ASSERT_FILE(stdout, "Draw\n");
}

TEST(P2_win) {
    INPUT_STRING(
        "6s Kh\n"
        "Ah 3h\n"
        "Ac 4h 5h Ks Jd\n"
    );

    app_main();
    ASSERT_FILE(stdout, "Player 2\n");
}

// Nonsense on input ends with failure

TEST(Error) {
    INPUT_STRING("poker\n");
    int retval = app_main();
    ASSERT(retval != 0);
}

// High card is identified correctly
TEST(P1_highcard) {
    INPUT_STRING(
        "Td As\n"
        "Th Kd\n"
        "8c 2h 6c Qs Jc\n"
    );

    app_main();
    ASSERT_FILE(stdout, "Player 1\n");
}

TEST(Draw_highcard) {
    INPUT_STRING(
        "7h 5c\n"
        "3d 8c\n"
        "Kh Qh 9h Ad Js\n"
    );

    app_main();
    ASSERT_FILE(stdout, "Draw\n");
}

TEST(P2_highcard) {
    INPUT_STRING(
        "4d Jc\n"
        "Td Qh\n"
        "5s 9c Kh 7h 6h\n"
    );

    app_main();
    ASSERT_FILE(stdout, "Player 2\n");
}
