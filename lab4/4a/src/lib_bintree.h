#include <stdlib.h>
#include <limits.h>

typedef struct BstNode BstNode;
typedef struct BstNode *BstNodePtr;
typedef struct Info_t *Info;

#define free_nullify(ptr) free (ptr); ptr = NULL;

struct BstNode {
    size_t height;
    Info info;
    BstNodePtr left, right;
    BstNodePtr par;
    BstNodePtr prev, next;
};

struct Info_t
{
    size_t key;
    size_t val;
};


#define NO_KEY __SIZE_MAX__

/*  Constructors.  */
BstNodePtr init_node (size_t key, size_t val);
Info new_info (size_t key, size_t val);

/*  Standard BST functions  */

size_t insert_bst (BstNodePtr *rooot, size_t key, size_t val);

/*  If key not found returns in par possible parent.  */
int find_by_key_or_ret_par (BstNodePtr *par, BstNodePtr rooot, size_t key);

BstNodePtr find_min (BstNodePtr rooot);
BstNodePtr find_max (BstNodePtr rooot);

void traverse_bst (BstNodePtr node, size_t key);

void print_bst (BstNodePtr rooot, size_t height);
void set_height (BstNodePtr *root);

int delete_bst (BstNodePtr *rooot, size_t key);

/*  Destructors.  */
void free_bst (BstNodePtr *root);