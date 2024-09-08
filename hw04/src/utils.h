#include <stdio.h>

#ifndef UTILS_H
#define UTILS_H

struct object
{
    int initialized;
    void (*destruct)(void *);
};

void object_destroy(void *);
void object_set_destructor(void *, void (*)(void *));
void object_avoid_duplicit_initialization(void *);

char *copy_string(const char *str);
char *trim_string(char *begin, char **end);
int empty_string(char *str);
int char_count(const char *str, char letter);
char *words_end(char *word);
char *get_token(char *string, int (*condition)(int));
char *read_line(FILE *input);

#endif
