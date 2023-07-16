#pragma once

#include "lib_bt.h"

#include <stddef.h>
#include <limits.h>
#include <string.h>


#define NODES_TO_FIND 0x10
#define NO_KEY __SIZE_MAX__

/*  TREE  */

/*  Insertion  */ 


/**
 * @brief Insert key-val pair to tree.
 * 
 * @param root [IN/OUT] - tree for insertion.
 * @param key  [IN] - key to insert.
 * @param val  [IN] - value to insert.
 */
void insert_bt (BNodePtr *root, Key key, Val val);

/*  insertion is always performed to the leaf  */
void insert_to_tree (BNodePtr *root, BNodePtr cnode, InfoListPtr info);

/*  recursive pushing  */
void split_node (BNodePtr *root, BNodePtr node);

/*  move down edge keys and make them children of the middle key  */
void split_middle (BNodePtr node, BNodePtr *left, BNodePtr *right);

/*  Search  */

/*  
    returns NULL if key not found; 
    the node and the position of the key in it otherwise 
*/
BNodePtr find_bt (BNodePtr root, Key key, size_t *pos);

BNodePtr find_max_node (BNodePtr root);
BNodePtr find_min_node (BNodePtr root);
int chld_for_descent (BNodePtr root, Key key);

/*  Print  */
void print_bt (BNodePtr root);
void set_height (BNodePtr root);
void print_bt_lvl (BNodePtr root, size_t height);
void colored_print_bt (BNodePtr root, Key key);
void colored_print_bt_lvl (BNodePtr root, size_t height, Key key);
void colored_print_node (BNodePtr root, Key key);

/*  Deletion  */
int delete_list_node (size_t ver, BNodePtr victim, size_t key_pos);

/*  deletion is always performed from the leaf  */
int delete_bt (BNodePtr *root, Key key, size_t ver);
void fix_after_del (BNodePtr *root, BNodePtr node);

size_t get_v3_num (BNodePtr par);

/*  redistribute  */
BNodePtr redistribute (BNodePtr leaf);

void rotate_right (size_t src, size_t dest, BNodePtr par);
void rotate_left (size_t src, size_t dest, BNodePtr par);
size_t set_leaf_num (BNodePtr par, BNodePtr leaf);

void insert_parent_key_to_chld (size_t victim_num, BNodePtr par);
void lshift_par_children (size_t victim_num, BNodePtr par);
void clear_par_and_leaf (size_t victim_num, BNodePtr par, BNodePtr leaf);
void insert_missing_child (size_t victim_num, BNodePtr par, BNodePtr leaf);

/*  merge - par of size 1 has two chidren of size 1  */
BNodePtr merge (BNodePtr *root, BNodePtr leaf);
void move_par_key_to_nonnull_chld (size_t victim_num, BNodePtr par);
void assign_grandchildren_to_nonnull_chld (size_t victim_num, BNodePtr par, BNodePtr leaf);
void clear_par_and_leaf_ (BNodePtr par, size_t victim_num);
BNodePtr get_nonnull_child (BNodePtr leaf);

/*  Traverse  */
void traverse_bt (BNodePtr root);

/*  Destructors  */
void free_bt (BNodePtr *root);



