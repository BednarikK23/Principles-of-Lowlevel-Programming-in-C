#include "structures.h"
#include "kids.h"
#include "utils.h"
#include "tree.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int init_tree(const char* path, node_s **root)
{
    /**
     * initializes tree, callocing memory for root, root->path and root->name
     * and copying path to root->path and root->name
     * returns 0 if successful
     */
    size_t base_size = 0;
    type_e base_type;
    if ((base_type = what_type(path, &base_size)) < 0) {
        return 1;
    }

    *root = calloc(1, sizeof(node_s));
    if (*root == NULL) {
        perror("calloc root");
        return 1;
    }

    (*root)->path = calloc(strlen(path) + 2, sizeof(char));
    if ((*root)->path == NULL) {
        free(*root);
        perror("calloc root->path");
        return 1;
    }

    (*root)->name = calloc(strlen(path) + 2, sizeof(char));
    if ((*root)->name == NULL) {
        free((*root)->path);
        free(*root);
        perror("calloc root->name");
        return 1;
    }

    strcpy((*root)->name, path);
    strcpy((*root)->path, path);
    (*root)->type = base_type;
    (*root)->size = base_size;
    (*root)->depth = 0;
    (*root)->error = 0;
    return 0;
}

void destroy_tree(node_s *node)
{
    /**
     * recursively destroys tree
     */
    if (node == NULL)
        return;

    for (int i = 0; i < node->kids.count; ++i) {
        destroy_tree(node->kids.children[i]);
    }

    if (node->kids.children != NULL)
        free(node->kids.children);

    free(node->name);
    free(node->path);
    free(node);
}
