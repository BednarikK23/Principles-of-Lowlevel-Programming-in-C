#include "kids.h"
#include "utils.h"
#include "structures.h"
#include "tree.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <stdint.h>
#include <errno.h>

#define UNITS_SIZE 6
#define UNIT 1024
const char *units[UNITS_SIZE] = {"B  ", "KiB", "MiB", "GiB", "TiB", "PiB"};

const char *pT = "|-- ";
const char *pI = "|   ";
const char *pL = "\\-- ";
const char *pN = "    ";
const char *pErr = "? ";
const char *pSuc = "  ";

int walk_tree(node_s *current, int flags)
{
    /**
     * Open directory and read it.
     */
    DIR *dir;
    if ((dir = opendir(current->path)) == NULL) {
        if (current->error == 0) {
            fprintf(stderr, "opendir %s: %s\n", current->path, strerror(errno));
            current->error = 1;
        }
        return 0;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL){
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        node_s *new_node = calloc(1, sizeof(node_s));
        if (new_node == NULL) {
            perror("calloc new_node");
            closedir(dir);
            return 1;
        }
        if (kids_care(current, new_node) != 0) {
            free(new_node); // only here free node cause node is not saved in tree
            closedir(dir);
            return 1;
        }
        new_node->error = 0;
        new_node->depth = current->depth + 1;

        new_node->name = NULL;
        if (make_name(entry->d_name, &new_node->name) != 0) {
            closedir(dir);
            return 1; // now node saved in kids will be taken care of by destroyer...
        }
        new_node->path = NULL;
        if (make_path(current->path, entry->d_name, &new_node->path) != 0) {
            closedir(dir);
            return 1;
        }

        new_node->type = get_type(new_node);

        new_node->size = get_size(flags, new_node);

        if (new_node->type == TYPE_DIR) {
            if (kids_init(new_node) != 0 || walk_tree(new_node, flags) != 0) {
                closedir(dir);
                return 1;
            }
            current->size += new_node->size;
        } else if (new_node->type == TYPE_FILE) {
            current->size += new_node->size;
        } else {
            destroy_last_node(current);
            continue;
        }

        current->error = current->error | new_node->error;
    }

    kids_done(current);

    closedir(dir);
    return 0;
}

int start_the_party(node_s *root, int flags)
{
    /**
     * init root node and start of collecting data
     * walk_tree() will take it from there and collect data we will need
     */
    root->depth = 0;
    root->error = 0;
    root->size = get_size(flags, root);

    if (root->type == TYPE_DIR) {
        if (kids_init(root) != 0 || walk_tree(root, flags) != 0) {
            return 1;
        }
    }

    return 0;
}

/**
 * --- end of collecting data ---
 *
 *
 * --- start of printing data ---
 */

void format_and_print(uint64_t size)
{
    double size_d = size;

    for (int i = 0; i < UNITS_SIZE; i++) {
        if (size_d < UNIT) {
            printf("%6.1f %s ", size_d, units[i]);
            return;
        }
        size_d /= UNIT;
    }
}

void print_size(uint64_t size, bool percents, uint64_t hundred_p)
{
    if (percents) {
        printf("%5.1f%% ", ((double) (size * 100) / hundred_p));
    } else {
        format_and_print(size);
    }
}

void print_tree(node_s *node, char *prefix, const char *curr, mode_s *modes)
{
    /*
     * go recursively through the tree and print nodes, add prefix to each node, depending on its depth
     * Depending on the flags (stored in modes) we will print the tree as user wants...
     * ...sorting kids, printing size, printing error, printing depth, printing percents
     */
    if (modes->limited_depth && node->depth > modes->depth) {
        return;
    }

    if (modes->error) {
        printf("%s", (node->error == 0) ? pSuc : pErr);
    }

    print_size(node->size, modes->percents, modes->hundred_p);

    printf("%s%s%s\n", prefix, curr, node->name);

    if (node->type != TYPE_DIR) {
        return;
    }

    char *new_prefix = calloc(strlen(prefix) + 8, sizeof(char));
    if (new_prefix == NULL) {
        perror("calloc new_prefix");
        return;
    }

    if (node->depth > 0) {
        if (strcmp(curr, pT) == 0) {
            strcpy(new_prefix, prefix);
            strcat(new_prefix, pI);
        } else if (strcmp(curr, pL) == 0) {
            strcpy(new_prefix, prefix);
            strcat(new_prefix, pN);
        }
    } else {
        strcpy(new_prefix, "");
    }

    sort_kids(node, modes->alpha_sort);

    for (int i = 0; i < node->kids.count - 1; i++) {
        print_tree(node->kids.children[i], new_prefix, pT, modes);
    }
    if (node->kids.count > 0) {
        print_tree(node->kids.children[node->kids.count - 1], new_prefix, pL, modes);
    }

    free(new_prefix);
}

/*
 * --- end of printing data ---
 *
 *
 * --- start of main ---
 */

int main(int argc, const char* argv[])
{
    uint64_t depth = 0;
    int mode;
    if ((mode = control_args(argc, argv, &depth)) == -1) {
        return 1;
    }

    node_s *root;
    if (init_tree(argv[argc - 1], &root) != 0) {
        return 1;
    }

    if (start_the_party(root, mode) != 0) {
        destroy_tree(root);
        return 1;
    }

    char* prefix = calloc(10, sizeof(char));
    if (prefix == NULL) {
        perror("calloc prefix");
        return 1;
    }
    prefix = strcpy(prefix, "");

    mode_s modes = {
            .alpha_sort = (mode & FLAG_S) == 0,
            .limited_depth = (mode & FLAG_D) != 0,
            .depth = depth,
            .percents = (mode & FLAG_P) != 0,
            .hundred_p = root->size,
            .error = root->error != 0,
    };

    print_tree(root, prefix, "", &modes);

    free(prefix);

    destroy_tree(root);
    return 0;
}
