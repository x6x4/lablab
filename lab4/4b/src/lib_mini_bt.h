#include "lib_bintree.h"

struct InfoNode {
    char *val;
    size_t ver;
    INode next;
};

/*  LIST  */
int change_info (InfoPtr *info, Key key, char *val);
int insert_to_ll (INode *head, char *val);
INode insert_to_ll_end (char *val, INode prev);
INode find_in_ll_by_val (char *val, INode head, INode *prev);
INode find_in_ll_by_ver (size_t ver, INode head, INode *prev);
void print_ll (INode head);
int delete_from_ll (INode *head, size_t ver);
void free_ll (INode *head);

/*  NODE  */

/*  Constructors  */
BNodePtr new_vertex (InfoPtr info);
BNodePtr new_bt_node (InfoPtr info, BNodePtr children[4], BNodePtr par);
InfoPtr new_info (Key key);

/*  Destructors  */
void free_vertex (BNodePtr *node);
void free_info (InfoPtr *info);

/*  Search  */
int find_in_vertex (BNodePtr node, char *key, size_t *pos);

/*  Reorder  */
void swap (InfoPtr *a, InfoPtr *b);
void asc_sort_2 (InfoPtr *a, InfoPtr *b);
void asc_sort_3 (InfoPtr *a, InfoPtr *b, InfoPtr *c);
void sort_vertex (BNodePtr node);

/*  Insertion  */ 
void insert_to_vertex (BNodePtr node, InfoPtr info);

/*  Deletion  */
int delete_from_vertex (BNodePtr node, Key key);

/*  Other  */
void fix_root_after_split (BNodePtr node, InfoPtr info, BNodePtr node1, BNodePtr node2);
Bool is_leaf (BNodePtr node);