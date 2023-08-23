#pragma once

#include "generic.h"
#include "lib_bintree.h"

/*  DIALOG  */

typedef struct dialog_strings *Dialog_info;

struct dialog_strings {
    const char *greeting;
    const char *msgs;
    size_t msgc;
    const char *exit_msg;
};

void dialog (Dialog_info info, int (*fptr[]) (BstNodePtr*, FILE*), BstNodePtr *bin_tree, FILE* file);


/*||||||||||||||||||||||||| < GENERAL FUNCTIONS > |||||||||||||||||||||||||*/

/*  Format of data file: <key1> <info1> ...  */
int import_tree (BstNodePtr *root, FILE *file);

int insert_tree (BstNodePtr *root, FILE *file);
int delete_tree (BstNodePtr *root, FILE *file);
int find_tree (BstNodePtr *root, FILE *file);
int findmin_tree (BstNodePtr *root, FILE *file);
int traverse_tree (BstNodePtr *root, FILE *file);
int print_tree (BstNodePtr *root, FILE *file);
int timing_tree (BstNodePtr *root, FILE *file);
int dump_tree (BstNodePtr *root, FILE *file);
void dump_node (BstNodePtr root, BstNodePtr node, FILE *file);

void timing_search (BstNodePtr test_root, int *test_keys, int bunch_num);
void timing_insertion (BstNodePtr *test_root, int *test_keys, int bunch_num);
void timing_deletion (BstNodePtr *test_root, int *test_keys, int bunch_num);
void timing_traversal (BstNodePtr test_root, int *test_keys, int bunch_num);
