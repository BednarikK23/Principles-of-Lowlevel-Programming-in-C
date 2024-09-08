#include "libs/cut.h"
#include "libs/utils.h"
#include "libs/mainwrap.h"

#include <string.h>

#include "../src/errors.h"
#include "../src/persons.h"

static void validate_person(struct persons *persons, const char *id, const char *name) {
    struct person *p = find_person(persons, id);
    ASSERT(p);
    CHECK(strcmp(p->name, name) == 0);
    CHECK(strcmp(p->id, id) == 0);
    CHECK(p->amount == 0);
}


TEST(persons_init) {
    struct persons persons;
    memset(&persons, 0, sizeof(persons));
    init_persons(&persons);

    CHECK(persons.size == 0);
    CHECK(persons.capacity == 16);

    object_destroy(&persons);
}

TEST(persons_add_find) {
    enum error_codes error_code;
    enum error_codes expected_code = SUCCESS;
    struct persons persons;
    memset(&persons, 0, sizeof(persons));
    init_persons(&persons);

    stack_top_frame();
    if ((error_code = read_error_point())) {
        object_destroy(&persons);
        if (error_code != expected_code) {
            DEBUG_MSG("Expected %d error code; %d given", expected_code, error_code);
            ASSERT(0);
        }
        return;
    }

    SUBTEST(add tanja) {
        add_person(&persons, "T", "Tanja");

        validate_person(&persons, "T", "Tanja");
    }
    SUBTEST(add boris and tanja) {
        add_person(&persons, "T", "Tanja");
        add_person(&persons, "Tx", "Boris");

        CHECK(persons.size == 2);

        validate_person(&persons, "T", "Tanja");
        validate_person(&persons, "Tx", "Boris");

        CHECK(find_person(&persons, "B") == NULL);
    }
    SUBTEST(add tanja 2x) {
        expected_code = PERSON_ALREADY_PRESENT;
        add_person(&persons, "T", "Tanja");
        add_person(&persons, "T", "Tanja");
    }
    SUBTEST(add wendy 2x valid) {
        add_person(&persons, "W", "Wendy");
        add_person(&persons, "WW", "Wendy");

        validate_person(&persons, "W", "Wendy");
        validate_person(&persons, "WW", "Wendy");
    }
    SUBTEST(add a lot of persons) {
        char id[5];

        for (int i = 0; i < 40; ++i) {
            sprintf(id, "x%.2dx", i);
            add_person(&persons, id, "Carl");
        }

        CHECK(persons.size == 40);
        CHECK(persons.capacity == 64);

        validate_person(&persons, "x06x", "Carl");
        validate_person(&persons, "x38x", "Carl");
    }
    exit_success();
}

