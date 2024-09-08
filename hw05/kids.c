#include "structures.h"
#include "kids.h"
#include "tree.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int kids_care(node_s *curr, node_s *new_node)
{
    /*
     * This function is used to add new node to children array of current node. If there is not enough space in array,
     * we realloc it to bigger size.
     */
    if (curr->kids.count >= curr->kids.allocated - 2) {
        node_s **tmp = realloc(curr->kids.children, 2 * curr->kids.allocated * sizeof(node_s*));
        if (tmp == NULL) {
            perror("realloc");
            return 1;
        }
        memset(tmp + curr->kids.allocated, 0, curr->kids.allocated * sizeof(node_s*));
        curr->kids.allocated *= 2;
        curr->kids.children = tmp;
    }

    curr->kids.children[curr->kids.count] = new_node;
    curr->kids.count++;

    return 0;
}

void destroy_last_node(node_s *curr)
{
    /*
     * This function is used to destroy last node in children array.
     * It is used when we find a type of file that we want to ignore - socket, named pipe...
     */
    if (curr->kids.count == 0) {
        return;
    }

    curr->kids.count--;
    destroy_tree(curr->kids.children[curr->kids.count]);
    curr->kids.children[curr->kids.count] = NULL;
}

int kids_init(node_s *node)
{
    /*
     * init of children array in node
     */
    node->kids.children = calloc(16, sizeof(node_s*));
    if (node->kids.children == NULL) {
        perror("calloc");
        return 1;
    }
    node->kids.allocated = 16;
    return 0;
}

void kids_done(node_s *curr)
{
    /**
     * This is just something to prevent from fragmentation of memory. Because rn we know that node dont have any more
     * children, we can realloc it to smaller size...
     */
    if (curr->kids.count < curr->kids.allocated - 2) {
        curr->kids.children = realloc(curr->kids.children, (curr->kids.count + 2) * sizeof(node_s*));
        return;
    }
}
