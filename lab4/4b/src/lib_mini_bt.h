#include <stdlib.h>

/*  Useful macros.  */

#define LT(s1,s2) ((strcmp (s1, s2)) <= 0)
#define GT(s1,s2) ((strcmp (s1, s2)) > 0)
#define EQ(s1,s2) ((strcmp (s1, s2)) == 0)

typedef struct BNode BNode;
typedef struct BNode *BNodePtr;

/*  List of values with the same key.  */
typedef struct InfoList *InfoListPtr;
typedef struct Info *InfoPtr;

typedef char *Key;
typedef short Bool;

#define KEYS_NUM 3
#define CHILD_NUM 4

/*  Tree structures.  */

struct BNode {
    size_t height;
    size_t csize;
    InfoListPtr info [KEYS_NUM];
    BNodePtr child [CHILD_NUM];
    BNodePtr par;
};

struct Info {
    char *val;
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

/*  LIST  */
int branch_ext (InfoListPtr *info, Key key, char *val);
int insert_to_ll (InfoPtr *head, char *val);
InfoPtr insert_to_ll_end (char *val, InfoPtr prev);
InfoPtr find_in_ll_by_val (char *val, InfoPtr head, InfoPtr *prev);
InfoPtr find_in_ll_by_ver (size_t ver, InfoPtr head, InfoPtr *prev);
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
Bool is_leaf (BNodePtr node);