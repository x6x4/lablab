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
};

struct InfoNode {
    char *val;
    size_t ver;
    INode next;
};

#define NO_KEY __SIZE_MAX__


/*  TREE  */

/*  Insertion  */ 

/*  recursive insertion is always performed to the leaf  */
void insert_bt (BNodePtr *root, BNodePtr cnode, InfoPtr info);
/*  recursive pushing  */
void split_up_from_node (BNodePtr *root, BNodePtr node);
/*  distribute the node's children equally between the created left and right nodes  */
void create_at_split (BNodePtr node, BNodePtr *left, BNodePtr *right);

int insert_to_ll (INode *head, char *val);
INode insert_to_ll_end (char *val, INode prev);
INode find_in_ll_by_val (char *val, INode head, INode *prev);
INode find_in_ll_by_ver (size_t ver, INode head, INode *prev);

/*  Search  */
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
int delete_from_ll (INode *head, size_t ver);
void fix_after_del (BNodePtr *root, BNodePtr node);
BNodePtr redistribute (BNodePtr leaf);
BNodePtr merge (BNodePtr *root, BNodePtr leaf);

/*  Destructors  */
void free_tree (BNodePtr root);


/*  NODE  */

/*  Constructors  */
BNodePtr new_vertex (InfoPtr info);
BNodePtr new_bt_node (InfoPtr info, BNodePtr children[4], BNodePtr par);
InfoPtr new_info (Key key, Key val);

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

