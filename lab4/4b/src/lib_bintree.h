#include <stdlib.h>
#include <limits.h>
#include <string.h>

typedef struct BNode BNode;
typedef struct BNode *BNodePtr;
typedef struct Info *InfoPtr;
typedef char *Key;
typedef short Bool;

#define KEYS_NUM 3
#define CHILD_NUM 4

#define LTE(s1,s2) ((strcmp (s1, s2)) <= 0)
#define GT(s1,s2) ((strcmp (s1, s2)) > 0)
#define EQ(s1,s2) ((strcmp (s1, s2)) == 0)

#define free_nullify(ptr) free (ptr); ptr = NULL;

struct BNode {
    size_t height;
    int csize;
    InfoPtr info [KEYS_NUM];
    BNodePtr child [CHILD_NUM];
    BNodePtr par;
};

struct Info
{
    Key key;
    char *val;
};

#define NO_KEY __SIZE_MAX__


/*  TREE  */

/*  INSERTION  */ 

/*  insertion is always performed to the leaf  */
void insert_bt (BNodePtr *root, BNodePtr cnode, InfoPtr info);
void split_vertex (BNodePtr *root, BNodePtr node);
/*  distribute the node's children equally between the created left and right nodes  */
void create_at_split (BNodePtr node, BNodePtr *left, BNodePtr *right);

BNodePtr search_bt (BNodePtr root, Key key, size_t *pos);

/*  DELETION  */
/*  deletion is always performed from the leaf  */
int delete_bt (BNodePtr *root, BNodePtr cnode, Key key);
BNodePtr fix_after_del (BNodePtr *root, BNodePtr node);

/*  DESTRUCTORS  */
void free_tree (BNodePtr root);

/*  NODE  */

/*  CONSTRUCTORS  */
BNodePtr new_vertex (InfoPtr info);
BNodePtr new_bt_node (InfoPtr info, BNodePtr children[4], BNodePtr par);
InfoPtr new_info (Key key, Key val);

/*  PRINT  */
void print_bt (BNodePtr root);
void set_height (BNodePtr root);
void print_bt_lvl (BNodePtr root, size_t height);

/*  DESTRUCTORS  */
void free_vertex (BNodePtr node);
void free_info (InfoPtr info);

/*  SEARCH  */
int find_in_vertex (BNodePtr node, char *key, size_t *pos);
size_t find_max_in_vertex (BNodePtr root);

/*  REORDER  */
void swap (InfoPtr *a, InfoPtr *b);
void asc_sort_2 (InfoPtr *a, InfoPtr *b);
void asc_sort_3 (InfoPtr *a, InfoPtr *b, InfoPtr *c);
void sort_vertex (BNodePtr node);

/*  INSERTION  */ 
void insert_to_vertex (BNodePtr node, InfoPtr info);

/*  DELETION  */
int delete_from_vertex (BNodePtr node, Key key);

/*  OTHER  */
void fix_root_after_split (BNodePtr node, InfoPtr info, BNodePtr node1, BNodePtr node2);
Bool is_leaf (BNodePtr node);

