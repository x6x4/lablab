#include <stdlib.h>
#include <limits.h>

typedef struct BinNode BinNode;
typedef struct BinNode *BinNodePtr;

struct BinNode {
    size_t height;
    size_t key;
    size_t val;
    BinNodePtr left, right;
    BinNodePtr par;
    BinNodePtr prev;
};

#define NO_KEY INT_MAX

int insert_bintree (BinNodePtr *rooot, size_t key, size_t val);
/*  Returns ERRWRG if key found and node with key in result, otherwise possible parent.  */
int find_by_key (BinNodePtr *result, BinNodePtr rooot, size_t key);
void traverse_bintree (BinNodePtr node, size_t key);
void print_bintree (BinNodePtr rooot, size_t height);

int is_left (BinNodePtr node);

int delete_bintree (BinNodePtr *rooot, size_t key);

void free_bintree (BinNodePtr root);