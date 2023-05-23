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





/*  NODE  */

/*  CONSTRUCTORS  */
BNodePtr new_vertex (Key key, Key val);
BNodePtr new_linked_vertex (InfoPtr info, BNodePtr children[4], BNodePtr par);
InfoPtr new_info (Key key, Key val);

/*  SEARCH  */
int search_in_node (BNodePtr node, char *key, size_t *pos);

/*  REORDER  */
void swap (InfoPtr *a, InfoPtr *b);
void asc_sort_2 (InfoPtr *a, InfoPtr *b);
void asc_sort_3 (InfoPtr *a, InfoPtr *b, InfoPtr *c);
void sort_node (BNodePtr node);

/*  INSERTION  */ 
void insert_to_node (BNodePtr node, InfoPtr info);

/*  DELETION  */
int delete_from_node (BNodePtr node, Key key);

/*  OTHER  */
void to_2vertex (BNodePtr node, InfoPtr info, BNodePtr node1, BNodePtr node2);
Bool is_leaf (BNodePtr node);

