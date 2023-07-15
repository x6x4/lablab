#pragma once

#include <stdlib.h>

/*  Useful macros.  */

#define LT(s1,s2) ((strcmp (s1, s2)) < 0)
#define GT(s1,s2) ((strcmp (s1, s2)) > 0)
#define EQ(s1,s2) ((strcmp (s1, s2)) == 0)

typedef struct BNode BNode;
typedef struct BNode *BNodePtr;

/*  List of values with the same key.  */
typedef struct InfoList *InfoListPtr;

typedef struct Info *InfoPtr;

typedef char *Key;
typedef char *Val;

typedef short Bool;

//  for code simplicity
#define KEYS_NUM 3
#define CHILD_NUM (KEYS_NUM + 1)

/*  Tree structures.  */

struct BNode {
    size_t height;
    size_t csize;
    InfoListPtr info [KEYS_NUM];
    BNodePtr child [CHILD_NUM];
    BNodePtr par;
};

/*  Node of the single-linked list of values with the same key.  */
struct Info {
    Val val;
    size_t ver;
    InfoPtr next;
};

/*  List of values with the same key.  */
struct InfoList
{
    Key key;
    InfoPtr head;
    size_t csize;
};


/*========================= < LIST > =========================*/

/**
* @brief Add value to list. Creates list if NULL. 
* 
* @param info [IN/OUT]  - list for insertion. 
    If it is NULL, creates list.
* @param key [IN]       - list key. 
    Required for list creation.
* @param val [IN]       - inserted value. Is copied. 
*/
void branch_ext (InfoListPtr *info, Key key, Val val);

void insert_to_ll (InfoPtr *head, Val val, InfoPtr prev);
InfoPtr find_in_ll_by_ver (InfoPtr head, size_t ver, InfoPtr *prev);
void print_ll (InfoPtr head);
int delete_from_ll (InfoPtr *head, size_t ver);
void free_ll (InfoPtr *head);

/*  NODE  */

/*  Constructors  */
BNodePtr new_vertex (InfoListPtr info);
BNodePtr new_bt_node (InfoListPtr info, BNodePtr children[4], BNodePtr par);
InfoListPtr new_infolist (Key key);

/*  Destructors  */
void free_vertex (BNodePtr *node);
void free_infolist (InfoListPtr *info);

/*  Search  */
int find_in_vertex (BNodePtr node, char *key, size_t *pos);

/*  Reorder  */
void swap (InfoListPtr *a, InfoListPtr *b);
void asc_sort_2 (InfoListPtr *a, InfoListPtr *b);
void asc_sort_3 (InfoListPtr *a, InfoListPtr *b, InfoListPtr *c);
void sort_node (BNodePtr node);

/*  Insertion  */ 
void insert_to_vertex (BNodePtr node, InfoListPtr info);

/*  Deletion  */
int shift_infolists_and_change_sz (BNodePtr node, Key key);

/*  Other  */
void construct_root_after_split (BNodePtr root, InfoListPtr root_info, BNodePtr left, BNodePtr right);

#define is_leaf(node) (!(node->child[0]))

/**
 * @brief Check validity of tree.
 * 
 * @param node  [IN] - tree to check.
 * @return char* - text of error. NULL if ok;
 */
const char* is_valid_node(const BNodePtr node);

#define VALIDATE_TREE(node)                                                         \
do {                                                                                \
    const char* _err = is_valid_node(node);                                         \
    if(_err) {                                                                      \
        fprintf(stderr, "Tree(%p) is INVALID: %s.\n", node, _err);                  \
        fprintf(stderr, "%s:%d %s\n ", __FILE__, __LINE__, __PRETTY_FUNCTION__);    \
        abort();                                                                    \
    }                                                                               \
} while(0)