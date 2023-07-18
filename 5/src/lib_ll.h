#pragma once

#include "lib_graph.h"
#include <string.h>

#define EQ(s1,s2) ((strcmp (s1, s2)) == 0)

typedef Vertex NodePtr;

/*||||||||||||||||||||||||| < LIST > |||||||||||||||||||||||||*/


/*  Single-linked list functions.  */

void insert_to_ll (NodePtr *head, NodePtr v, NodePtr prev);
NodePtr find_in_ll (NodePtr head, char *name, NodePtr *prev);
void print_ll (NodePtr head);
void print_node (NodePtr node, NodePtr head);
void print_node_weight (NodePtr node);
int delete_from_ll (NodePtr *head, char *name);
void free_ll (NodePtr *head);
