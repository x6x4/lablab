#include <stdlib.h>
#include <limits.h>

typedef struct BNode BNode;
typedef struct BNode *BNodePtr;
typedef struct Info *Info;

#define KEYS_NUM 2
#define CHILD_NUM 3

struct BNode {
    size_t height;
    Info info [KEYS_NUM];
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
Info new_info (char *key, char *val);

/*  
    Returns node with key and key position in this node in success.
    Otherwise - place to insert to. 

    Three cases: 
    key found (ERRSUC); 
    there is free place for key (ERRFREE); 
    there is no free place for key (ERRFULL);
*/
int search_bt (BNodePtr rooot, char *key, size_t *pos, BNodePtr *node);

int insert_bt (BNodePtr *rooot, char *key, char *val);
int cond_insert (BNodePtr *root, char *key, char *val, size_t *pos, BNodePtr *node);
