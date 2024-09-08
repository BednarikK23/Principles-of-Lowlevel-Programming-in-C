#ifndef HW05_STRUCTURES_H
#define HW05_STRUCTURES_H

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#define BLOCK_SIZE 512
#define FLAG_A 1
#define FLAG_S 2
#define FLAG_D 4
#define FLAG_P 8


struct node;

typedef enum type {
    ERROR = -2,
    TYPE_UNKNOWN = -1,
    TYPE_DIR = 0,
    TYPE_FILE = 1,
} type_e;


typedef struct kids {
    struct node **children;
    int count;
    int allocated;
} kids_s;

typedef struct node {
    char* name;
    char* path;
    type_e type;
    uint64_t size;
    size_t depth;
    kids_s kids;
    int error;
} node_s;

typedef struct mode {
    bool alpha_sort;
    bool limited_depth;
    uint64_t depth;
    bool percents;
    uint64_t hundred_p;
    bool error;
} mode_s;

#endif //HW05_STRUCTURES_H
