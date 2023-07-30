#pragma once

#include <stdlib.h>


/*  Useful macros.  */

#define LT(s1,s2) ((strcmp (s1, s2)) < 0)
#define GT(s1,s2) ((strcmp (s1, s2)) > 0)
#define EQ(s1,s2) ((strcmp (s1, s2)) == 0)

#define KEYS_NUM 3
#define CHILD_NUM (KEYS_NUM + 1)


/*  Useful typedefs.  */

typedef char *Key;
typedef char *Val;

typedef short Bool;



/*||||||||||||||||||||||||| < TREE STRUCTURES > |||||||||||||||||||||||||*/


typedef struct BNode BNode;
typedef struct BNode *BNodePtr;
typedef struct InfoList *InfoListPtr;


struct BNode {
    size_t height;
    size_t csize;
    InfoListPtr info [KEYS_NUM];
    BNodePtr child [CHILD_NUM];
    BNodePtr par;
};


/*  List of values with the same key.  */
typedef struct Info *InfoPtr;

struct InfoList
{
    Key key;
    InfoPtr head;
    size_t csize;
};


/*  Node of the single-linked list of values with the same key.  */

struct Info {
    Val val;
    size_t ver;
    InfoPtr next;
};



/*||||||||||||||||||||||||| < LIST > |||||||||||||||||||||||||*/


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


/*  Single-linked list functions.  */

void insert_to_ll (InfoPtr *head, Val val, InfoPtr prev);
InfoPtr find_in_ll_by_ver (InfoPtr head, size_t ver, InfoPtr *prev);
void print_ll (InfoPtr head);
int delete_from_ll (InfoPtr *head, size_t ver);
void free_ll (InfoPtr *head);



/*||||||||||||||||||||||||| < NODE > |||||||||||||||||||||||||*/


/*  Constructors  */

BNodePtr new_vertex (InfoListPtr info);
BNodePtr new_bt_node (InfoListPtr info, BNodePtr children[4], BNodePtr par);
InfoListPtr new_infolist (Key key);
void new_root_from_fields (BNodePtr root, InfoListPtr root_info, BNodePtr left, BNodePtr right);


/*  Destructors  */

void free_vertex (BNodePtr *node);
void free_infolist (InfoListPtr *info);


/*  Deletion  */

int shift_infolists_and_change_sz (BNodePtr node, Key key);


/*  Search  */

int find_in_vertex (BNodePtr node, char *key, size_t *pos);


/*  Reorder  */

void swap (InfoListPtr *a, InfoListPtr *b);
void asc_sort_2 (InfoListPtr *a, InfoListPtr *b);
void asc_sort_3 (InfoListPtr *a, InfoListPtr *b, InfoListPtr *c);
void sort_node (BNodePtr node);


/*  Insertion  */ 
void insert_to_vertex (BNodePtr node, InfoListPtr info);



/*||||||||||||||||||||||||| < VALIDATION > |||||||||||||||||||||||||*/


#define is_leaf(node) (!(node->child[0]))


#define try(check)                  \
do {                                \
    const char* err = check;        \
    if (err) return err;            \
} while(0)    


/**
 * @brief Check validity of infolist.
 * 
 * @param info  [IN] - infolist to check.
 * @return char* - text of error. NULL if ok;
 */
//static const char* is_valid_info (const InfoListPtr info);

/**
 * @brief Check validity of tree.
 * 
 * @param node  [IN] - tree to check.
 * @return char* - text of error. NULL if ok;
 */
const char* is_valid_node (const BNodePtr node);

const char* check_sz (BNodePtr node);
const char* check_node_infos (BNodePtr node);
const char* check_node_children (BNodePtr node);


#define VALIDATE_TREE(node)                                                         \
do {                                                                                \
    const char* _err = is_valid_node (node);                                         \
    if(_err) {                                                                      \
        fprintf(stderr, "Tree(%p) is INVALID: %s.\n", node, _err);                  \
        fprintf(stderr, "%s:%d %s\n ", __FILE__, __LINE__, __PRETTY_FUNCTION__);    \
        abort();                                                                    \
    }                                                                               \
} while(0)


