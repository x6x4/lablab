#include <stdlib.h>
#include <limits.h>

typedef struct BstNode BstNode;
typedef struct BstNode *BstNodePtr;

struct BstNode {
    size_t height;
    size_t key;
    size_t val; // todo info
    BstNodePtr left, right;
    BstNodePtr par;
    BstNodePtr prev;
};

#define NO_KEY __SIZE_MAX__

size_t insert_bst (BstNodePtr *rooot, size_t key, size_t val);
/*  If key not found puts in result possible parent.  */
int find_by_key (BstNodePtr *result, BstNodePtr rooot, size_t key);

BstNodePtr find_min (BstNodePtr rooot);
BstNodePtr find_max (BstNodePtr rooot);

BstNodePtr next_node (BstNodePtr root, BstNodePtr node);
void traverse_bst (BstNodePtr node, size_t key);

void print_bst (BstNodePtr rooot, size_t height);
void set_height (BstNodePtr *root);

int delete_bst (BstNodePtr *rooot, size_t key);

void free_bst (BstNodePtr root);