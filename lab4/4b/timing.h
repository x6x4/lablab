#pragma once

#include "src/lib_bt_func.h"
#include "src/generic.h"

/*  Killed on 1e4  */
#define BUNCH_SZ (int) 10

/*  Overall EPOCHS_NUM epochs with EPOCHS_NUM bunches in each.  */
#define EPOCHS_NUM 10

int timing_tree ();

void timing_search (BNodePtr test_root, char test_keys[BUNCH_SZ*EPOCHS_NUM][1], int bunch_num);
void timing_insertion (BNodePtr *test_root, char test_keys[BUNCH_SZ*EPOCHS_NUM][1], int bunch_num);
void timing_deletion (BNodePtr *test_root, char test_keys[BUNCH_SZ*EPOCHS_NUM][1], int bunch_num);
void timing_traversal (BNodePtr test_root, char test_keys[BUNCH_SZ*EPOCHS_NUM][1], int bunch_num);

void silent_traverse_bt (BNodePtr root);