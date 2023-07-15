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
/**
 * @brief Deletes BNode. Do NOT frees children.
 * 
 * @param node [IN/OUR] - node to free. It becomes NULL after clearing.
 */
void free_vertex (BNodePtr *node);

/**
 * @brief Deletes InfoList.
 * 
 * @param info [IN/OUR] - list to free. It becomes NULL after clearing.
 */
void free_infolist (InfoListPtr *info);

/*  Search  */
/**
 * @brief Searches if key exists in node.
 * 
 * @param node [IN]           - node to search in.
 * @param key  [IN]           - key to search for.
 * @param pos  [OUT] OPTIONAL - position of key. Not changed if not found.
 * @return int - ERRSUC if exists / ERRWRG if not or bad input.
 */
int find_in_vertex (BNodePtr node, Key key, size_t *pos);

/*  Reorder  */
/**
 * @brief Swaps two InfoLists.
 * 
 * @param a [IN/OUT] -+-> Not NULL pointers to info lists.
 * @param b [IN/OUT] -+
 */
void swap (InfoListPtr *a, InfoListPtr *b);

/**
 * @brief Sorts two InfoLists by key.
 * 
 * @param a [IN/OUT] -+-> Not NULL pointers to info lists.
 * @param b [IN/OUT] -+
 */
void asc_sort_2 (InfoListPtr *a, InfoListPtr *b);

/**
 * @brief Sorts three InfoLists by key.
 * 
 * @param a [IN/OUT] -+-> Not NULL pointers to info lists. Lists must be NOT NULL too. 
 * @param b [IN/OUT] -+
 * @param c [IN/OUT] -+
 */
void asc_sort_3 (InfoListPtr *a, InfoListPtr *b, InfoListPtr *c);

/**
 * @brief Sorts infos by key in node.
 *                              ^
 * @param node [IN] ------------+
 */
void sort_node (BNodePtr node);

/*  Insertion  */ 
/**
 * @brief Inserts new key (as InfoList) to vertex. Must be free space in vertex.
 * 
 * @param node [IN] - node insert to.
 * @param info [IN] - info insert to. Takes ownership.
 */
void insert_to_vertex (BNodePtr node, InfoListPtr info);

/*  Deletion  */
/**
 * @brief Corrects list in node after deletion.
 * 
 * @param node [IN] - node to fixup. 
 * @param key  [IN] - ??
 * @return int - error code. Possible: ERRWRG
 */
int shift_infolists_and_change_sz (BNodePtr node, Key key);

/*  Other  */

/**
 * @brief Builds new root after split.
 * 
 * @param root      [IN] - new root vertex to fill. Not NULL.
 * @param root_info [IN] - new root info.
 * @param left      [IN] - left child.
 * @param right     [IN] - right child.
 */
void construct_root_after_split (BNodePtr root, InfoListPtr root_info, BNodePtr left, BNodePtr right);

/**
 * @brief Check if node is leaf.
 * 
 * @param node [IN] - not NULL node to check.
 * @return Bool - if is leaf.
 */
Bool is_leaf (BNodePtr node);

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
