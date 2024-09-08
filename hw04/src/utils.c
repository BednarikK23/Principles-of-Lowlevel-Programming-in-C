#include "utils.h"

#include "errors.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void object_destroy(void *o)
{
    struct object *object = (struct object *) o;

    object->destruct && (object->destruct(o), 1);
}

void object_set_destructor(void *o, void (*destructor)(void *))
{
    struct object *object = (struct object *) o;

    object->destruct = destructor;
    object->initialized = 1;
}

void object_avoid_duplicit_initialization(void *o)
{
    struct object *object = (struct object *) o;

    OP(!object->initialized, DUPLICIT_INITIALIZATION);
}

char *copy_string(const char *str)
{
    char *copy;
    OP(copy = (char *) malloc(strlen(str) + 1), ALLOCATION_FAILED);
    strcpy(copy, str);
    return copy;
}

char *trim_string(char *begin, char **end)
{
    while (*begin && isspace(*begin))
        ++begin;
    if (end) {
        char *last_char = *end = begin;
        while (**end) {
            if (!isspace(**end))
                last_char = *end;
            ++(*end);
        }
        *end = last_char;
        if (**end && !isspace(**end))
            ++(*end);
    }
    return begin;
}

int empty_string(char *str)
{
    char *end;
    str = trim_string(str, &end);
    return str == end;
}

char *words_end(char *string)
{
    return get_token(string, isspace);
}

char *get_token(char *string, int (*condition)(int))
{
    while (*string && !condition(*string))
        ++string;
    return string;
}

int char_count(const char *string, char letter)
{
    int result = 0;
    for (; *string; ++string) {
        if (*string == letter)
            ++result;
    }
    return result;
}

char *read_line(FILE *input)
{
    int capacity = 16;
    int size = 0;
    char *buffer = (char *) malloc(capacity);
    int c;

    while ((c = fgetc(input)) != EOF && c != '\n') {
        if (size == capacity) {
            capacity *= 2;
            char *tmp = (char *) realloc(buffer, capacity);
            if (!tmp) {
                free(buffer);
                error_happened(ALLOCATION_FAILED);
            }
            buffer = tmp;
        }
        buffer[size++] = c;
    }
    buffer[size] = '\0';
    return buffer;
}
