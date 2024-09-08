#include "errors.h"

#include <stdio.h>

static jmp_buf *point;

void set_error_point(jmp_buf *current)
{
    point = current;
}

jmp_buf *get_error_point()
{
    return point;
}

__attribute__((noreturn))
int error_happened(enum error_codes code)
{
    longjmp(*point, code);
}

__attribute__((noreturn))
void exit_success()
{
    error_happened(SUCCESS);
}

static const char *resolve_message(enum error_codes code)
{
    static char unknownError[128];
    switch (code) {
    case INITAL_PASS:
    case SUCCESS:
        return "OK";
    case DUPLICIT_INITIALIZATION:
        return "duplicit initialization";
    case ALLOCATION_FAILED:
        return "allocation failed";
    case CURRENCY_ALREADY_PRESENT:
        return "currency is already present";
    case CURRENCY_DUPLICATE_MAIN:
        return "duplicated main currency";
    case CURRENCY_NOT_FOUND:
        return "currency not found";
    case PERSON_ALREADY_PRESENT:
        return "person already present";
    case INVALID_ARGUMENTS:
        return "invalid arguments; use <program> <person-file> <currency-file> <payments-file>";
    default:
        sprintf(unknownError, "unknown error (%u)", code);
        return unknownError;
    }
}

int return_code(enum error_codes code)
{
    if (code == SUCCESS)
        return 0;
    return code;
}

void print_error_message(enum error_codes code)
{
    fprintf(stderr, "Error happened: %s", resolve_message(code));
}
