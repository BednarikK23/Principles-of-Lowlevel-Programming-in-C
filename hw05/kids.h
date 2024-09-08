#include "structures.h"

#ifndef HW05_KIDS_H
#define HW05_KIDS_H

void destroy_last_node(node_s *curr);
int kids_care(node_s *curr, node_s *new_node);
int kids_init(node_s *node);
void kids_done(node_s *curr);

#endif //HW05_KIDS_H
