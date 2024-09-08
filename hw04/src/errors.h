#include <setjmp.h>

#ifndef ERRORS_H
#define ERRORS_H

enum error_codes
{
    INITAL_PASS = 0,
    SUCCESS,

    DUPLICIT_INITIALIZATION,
    ALLOCATION_FAILED,
    CURRENCY_ALREADY_PRESENT,
    CURRENCY_DUPLICATE_MAIN,
    CURRENCY_NOT_FOUND,
    PERSON_ALREADY_PRESENT,
    PERSON_NOT_FOUND,

    INVALID_ARGUMENTS
};

#define stack_top_frame() jmp_buf current_frame; enum error_codes frame_code; set_error_point(&current_frame)
#define stack_frame() jmp_buf current_frame, *top_frame = get_error_point(); enum error_codes frame_code;set_error_point(&current_frame)
#define read_error_point() (frame_code = (enum error_codes)setjmp(*get_error_point()))
#define leave(value) do { set_error_point(top_frame); if (frame_code == INITAL_PASS || frame_code == SUCCESS) return value; error_happened(frame_code); } while(0)

#define OP(DO, ERR) (void)((DO) || error_happened(ERR))

void print_error_message(enum error_codes code);
int return_code(enum error_codes code);
void set_error_point(jmp_buf *point);
jmp_buf *get_error_point();
__attribute__((noreturn)) int error_happened(enum error_codes code);
__attribute__((noreturn)) void exit_success();

#endif
