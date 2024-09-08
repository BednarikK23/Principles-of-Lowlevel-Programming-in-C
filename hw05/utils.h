#include "structures.h"

#include <stdbool.h>
#include <stdlib.h>

#ifndef HW05_UTILS_H
#define HW05_UTILS_H

int control_args(int argc, const char* argv[], uint64_t *depth);
type_e what_type(const char* path, size_t *size);
type_e get_type(node_s* node);
size_t get_size(int flags, node_s *node);
int make_path (const char *path, const char *name, char **new_path);
int make_name(const char *tmp, char **name);
void sort_kids(node_s *node, bool alpha_sort);

#endif //HW05_UTILS_H
