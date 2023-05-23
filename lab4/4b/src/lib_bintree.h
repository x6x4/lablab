#include <stdlib.h>
#include <limits.h>

typedef struct BNode BNode;
typedef struct BNode *BNodePtr;
typedef struct Info *InfoPtr;

#define KEYS_NUM 3
#define CHILD_NUM 4

struct BNode {
    size_t height;
    InfoPtr info [KEYS_NUM];
    BNodePtr child [CHILD_NUM];
    BNodePtr par;
};

struct Info
{
    char *key;
    char *val;
};


#define NO_KEY __SIZE_MAX__

BNodePtr init_node (char *key, char *val);
InfoPtr new_info (char *key, char *val);
void free_info (InfoPtr info);

int split_leaf (BNodePtr *root, BNodePtr node, char *key, char *val);

/*  
    Returns node with key and key position in this node in success.
    Otherwise - place to insert to. 

    Four cases: 
    key found (ERRSUC); 
    there is free place for key (ERRFREE); 
    there is no free place for key (ERRFULL);
    there is void child for key (ERRNEW)
*/
int search_bt (BNodePtr rooot, char *key, size_t *pos, BNodePtr *node);

int insert_bt (BNodePtr *rooot, char *key, char *val);
int cond_insert (BNodePtr *root, char *key, char *val, size_t *pos, BNodePtr *node);

void set_height (BNodePtr *root);
void print_bt (BNodePtr root, size_t height);