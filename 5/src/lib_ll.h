#pragma once

#include "lib_graph.h"
#include <string.h>

#define EQ(s1,s2) ((strcmp (s1, s2)) == 0)

typedef V_node NodePtr;
typedef Edge InfoPtr;

/*||||||||||||||||||||||||| < LIST > |||||||||||||||||||||||||*/


/*  Single-linked list functions.  */

void insert_to_ll (NodePtr *head, NodePtr v, NodePtr prev);
NodePtr find_in_ll (NodePtr head, char *name, NodePtr *prev);
void print_ll (NodePtr head);
void print_node (NodePtr node);
void print_node_edge (NodePtr node);
int delete_from_ll (NodePtr *head, char *name, InfoPtr *info_to_delete);
void free_ll (NodePtr *head, char *head_name);
