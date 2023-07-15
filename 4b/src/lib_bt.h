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

/*  Node of the singe-linked list of values with the same key.  */
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

/*=====================================================< LIST >=================================================*/

/**
 * @brief Add UNIQUE value to list. Creates list if NULL.
 * 
 * @param info [IN/OUT] - list insert to. If NULL creates list.
 * @param key  [IN]     - key of list. Needed for creation.
 * @param val  [IN]     - value insert to. Copied. If value already exists returns ERRDUP.
 * @return int - Error code. Possible ERRDUP - value already exists.
 */
int branch_ext (InfoListPtr *info, Key key, Val val);

/**
 * @brief Insert val into list after prev 
 * 
 * @param head [IN/OUT] - head of list
 * @param val  [IN]     - value to insert. Copies it to node.
 * @param prev [IN]     - element to insert after. NULL means insert to head.
 */
void insert_to_ll (InfoPtr *head, Val val, InfoPtr prev);

/**
 * @brief Finds first node in list with value equal to val 
 * 
 * @param head [IN]           - head of list
 * @param val  [IN]           - value compare to
 * @param prev [OUT] OPTIONAL - previous element in list. May be useful for insertion. End of list if not found.
 * @return InfoPtr - first element with same value. NULL if not found.
 */
InfoPtr find_in_ll_by_val (InfoPtr head, Val val, InfoPtr *prev);

/**
 * @brief Finds first node in list with value equal to val 
 * 
 * @param head [IN]           - head of list
 * @param ver  [IN]           - version to search for
 * @param prev [OUT] OPTIONAL - previous element in list. May be useful for insertion. End of list if not found.
 * @return InfoPtr - first element with same value. NULL if not found.
 */
InfoPtr find_in_ll_by_ver (InfoPtr head, size_t ver, InfoPtr *prev);

/**
 * @brief Pretty prints list.
 * 
 * @param head [IN] - head of list.
 */
void print_ll (InfoPtr head);

/**
 * @brief Delete element with given version from list.
 *                                      ^
 * @param head [IN/OUT] - head of list  |
 * @param ver  [IN]     ----------------+
 * @return int - Error code. Possible: ERRWRG - no such elem in list.
 */
int delete_from_ll (InfoPtr *head, size_t ver);

/**
 * @brief Deletes & frees whole list.
 * 
 * @param head [IN/OUT] - head of list. Sets to NULL.
 */
void free_ll (InfoPtr *head);

/*=====================================================< NODE >=================================================*/

/*  Constructors  */

/**
 * @brief Creates new standalone node for tree.
 * 
 * @param info [IN] - content for node (Key, list of values). Takes ownership.
 * @return BNodePtr - created node. NULL if any error occurred.
 */
BNodePtr new_vertex (InfoListPtr info);

/**
 * @brief Creates new node for tree with given children and parent.
 * 
 * @param info      [IN] - content  for node (Key, list of values). Takes ownership.
 * @param children  [IN] - children for node. Takes ownership.
 * @param par       [IN] - parent of node
 * @return BNodePtr - created node. NULL if any error occurred.
 */
BNodePtr new_bt_node (InfoListPtr info, BNodePtr children[4], BNodePtr par);

/**
 * @brief Creates new info list for given key.
 * 
 * @param key [IN] - key list is made for.
 * @return InfoListPtr - created list. NULL if any error occurred.
 */
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
Bool is_leaf (BNodePtr node);