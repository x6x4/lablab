#include <stddef.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

typedef struct BNode BNode;
typedef struct BNode *BNodePtr;
typedef struct Info *InfoPtr;
typedef struct InfoNode *INode;
typedef char *Key;
typedef short Bool;

#define KEYS_NUM 3
#define CHILD_NUM 4

#define LTE(s1,s2) ((strcmp (s1, s2)) <= 0)
#define GT(s1,s2) ((strcmp (s1, s2)) > 0)
#define EQ(s1,s2) ((strcmp (s1, s2)) == 0)

#define free_nullify(ptr) free (ptr); ptr = NULL;

#define NODES_TO_FIND 0x10

struct BNode {
    size_t height;
    size_t csize;
    InfoPtr info [KEYS_NUM];
    BNodePtr child [CHILD_NUM];
    BNodePtr par;
};

struct Info
{
    Key key;
    INode head;
    size_t csize;
};

#define NO_KEY __SIZE_MAX__


/*  TREE  */

/*  Insertion  */ 

/*  
    insert in list if duplicate key;
    calls insert_bt otherwise 
*/
int insert_bt (BNodePtr *root, Key key, char *val);

/*  recursive insertion is always performed to the leaf  */
void insert_to_tree (BNodePtr *root, BNodePtr cnode, InfoPtr info);

/*  recursive pushing  */
void split_up_from_node (BNodePtr *root, BNodePtr node);
/*  distribute the node's children equally between the created left and right nodes  */
void create_at_split (BNodePtr node, BNodePtr *left, BNodePtr *right);

/*  Search  */
/*  returns NULL if key not found  */
BNodePtr find_bt (BNodePtr root, Key key, size_t *pos);
BNodePtr find_max (BNodePtr root);

/*  Print  */
void print_bt (BNodePtr root);
void set_height (BNodePtr root);
void print_bt_lvl (BNodePtr root, size_t height);
void colored_print_bt (BNodePtr root, Key key);
void colored_print_bt_lvl (BNodePtr root, size_t height, Key key);

/*  Deletion  */
/*  deletion is always performed from the leaf  */
int delete_bt (BNodePtr *root, Key key, size_t ver);
void fix_after_del (BNodePtr *root, BNodePtr node);

size_t set_v3_num (BNodePtr par, BNodePtr leaf);

/*  redistribute  */
BNodePtr redistribute (BNodePtr leaf);
void insert_parent_key_to_chld (size_t num, BNodePtr par);
void lshift_par_children (size_t num, BNodePtr par);
void clear_par_and_leaf (size_t num, BNodePtr par, BNodePtr leaf);
void insert_missing_child (size_t num, BNodePtr par, BNodePtr leaf);

/*  merge - par has two chidren of size 1  */
BNodePtr merge (BNodePtr *root, BNodePtr leaf);
void move_par_key_to_chld (size_t num, BNodePtr par);
void move_grandchildren (size_t num, BNodePtr par, BNodePtr leaf);
void clear_par_and_leaf_ (BNodePtr par, size_t num);
BNodePtr get_nonnull_child (BNodePtr leaf);

/*  Destructors  */
void free_bt (BNodePtr *root);



