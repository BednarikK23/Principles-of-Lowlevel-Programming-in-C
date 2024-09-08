#include "utils.h"
#include "kids.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

int make_path (const char *path, const char *name, char **new_path)
{
    /**
     * makes path. Callocing memory for new_path, copying path to new_path, adding '/' to the end of new_path if it's not there
     * and adding name to the end of new_path. Returns 0 if successful calloc.
     * */
    *new_path = calloc(strlen(path) + strlen(name) + 3, sizeof(char));
    if (*new_path == NULL) {
        perror("calloc new_path");
        return 1;
    }

    strcpy(*new_path, path);
    if ((*new_path)[strlen(*new_path) - 1] != '/') {
        strcat(*new_path, "/");
    }
    strcat(*new_path, name);

    return 0;
}

int make_name(const char *tmp, char **name)
{
    /**
     * makes name. Callocing memory for name, copying tmp to name and returning 0 if successful calloc.
     * */
    *name = calloc(strlen(tmp) + 2, sizeof(char));
    if (*name == NULL) {
        perror("calloc name");
        return 1;
    }
    strcpy(*name, tmp);
    return 0;
}

size_t get_size(int flags, node_s *node)
{
    /*
     * returns size of node, using lstat, applicable on node
     * if node is a directory, returns size of all files in directory,
     * if node is a file, returns size of file,
     * if node is neither, returns 0
     */
    const char* name = node->path;
    struct stat st;
    if (lstat(name, &st) != 0) {
        if (node->error == 0) {
            perror("lstat, for size");
            node->error = 1;
            return 0;
        }
    }

    if ((flags & FLAG_A) == 0) {
        return st.st_blocks * BLOCK_SIZE;
    }
    return st.st_size;
}

type_e what_type(const char* path, size_t *size)
{
    /*
     * returns type of node, using lstat, applicable on path (root using in init_tree)
     * if node is a directory, returns TYPE_DIR,
     * if node is a file, returns TYPE_FILE,
     * if node is neither, returns TYPE_UNKNOWN
     */
    struct stat st;
    if (lstat(path, &st) == -1) {
        perror("stat");
        return ERROR;
    }

    if (S_ISDIR(st.st_mode)) {
        return TYPE_DIR;
    }
    if (S_ISREG(st.st_mode)) {
        *size = st.st_size;
        return TYPE_FILE;
    }

    return TYPE_UNKNOWN;
}

type_e get_type(node_s *node)
{
    /*
     * returns type of node, using lstat, applicable on node->path
     * if node is a directory, returns TYPE_DIR,
     * if node is a file, returns TYPE_FILE,
     * if node is neither, returns TYPE_UNKNOWN
     */
    const char* path = node->path;
    struct stat st;
    if (lstat(path, &st) == -1) {
        if (node->error == 0) {
            perror("stat");
            node->error = 1;
            return ERROR;
        }
    }

    if (S_ISDIR(st.st_mode)) {
        return TYPE_DIR;
    }
    if (S_ISREG(st.st_mode)) {
        node->size = st.st_size;
        return TYPE_FILE;
    }

    return TYPE_UNKNOWN;
}

int control_args(int argc, const char* argv[], uint64_t *depth)
{
    /**
     * checks validity of arguments, if invalid, prints error message and returns -1. 0 on success.
     * */

    int flags = 0;
    if (argc < 2 || argc > 7) {
        fprintf(stderr,"Usage: %s <path>\n", argv[0]);
        return -1;
    }

    if (argc == 2) {
        return 0;
    }

    for (int i = 1; i < argc - 1; i++) {
        if (strcmp(argv[i], "-a") == 0) {
            if ((flags & FLAG_A) != 0) {
                fprintf(stderr, "Duplicate -a.\n");
                return -1;
            }

            flags ^= FLAG_A;
        } else if (strcmp(argv[i], "-s") == 0) {
            if ((flags & FLAG_S) != 0) {
                fprintf(stderr, "Duplicate -s.\n");
                return -1;
            }
            flags ^= FLAG_S;

        } else if (strcmp(argv[i], "-p") == 0) {
            if ((flags & FLAG_P) != 0) {
                fprintf(stderr, "Duplicate -p.\n");
                return -1;
            }

            flags ^= FLAG_P;
        } else if (strcmp(argv[i], "-d") == 0) {
            if ((flags & FLAG_D) != 0) {
                fprintf(stderr, "Duplicate -d.\n");
                return -1;
            }

            flags ^= FLAG_D;

            char *end;
            ++i;
            int64_t d = strtol(argv[i], &end, 10);
            if (d < 0 || *end != '\0') {
                fprintf(stderr, "After depth must be number greater or equal than 0\n");
                return -1;
            }
            *depth = d;
        } else {
            fprintf(stderr, "Usage: %s <path> [-a] [-b] [-m] [-h] [-d <depth>]\n", argv[0]);
            return -1;
        }
    }

    return flags;
}

int compare_alpha(const void *a, const void *b)
{
    /*
     * compares two nodes by their names, but idk why it does not work properly... (test are not ok)
     */
    node_s *node_a = *(node_s **) a;
    node_s *node_b = *(node_s **) b;

    int res = strcasecmp(node_a->name, node_b->name);
    if (res != 0) {
        return res;
    }

    return (strcmp(node_a->name, node_b->name)) * -1;
}

int compare_size(const void *a, const void *b)
{
    /**
     * compares two nodes by their sizes
     * returns 1 if a < b
     * */
    node_s *node_a = *(node_s **) a;
    node_s *node_b = *(node_s **) b;

    return node_b->size - node_a->size;
}

void sort_kids(node_s *node, bool alpha)
{
    qsort(node->kids.children, node->kids.count, sizeof(node_s *),
          (alpha) ? &compare_alpha : &compare_size);
}

