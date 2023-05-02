#include <stdlib.h>

typedef struct table table_t;
typedef struct keyspace ks_t;
typedef struct info info_t;
typedef struct node node_t;


/*  Main structure.  */
struct table {
    size_t sz;

    ks_t *kslist;
    size_t kslist_max_sz;
    size_t kslist_sz;
};

/*  Linked list of items with same key.  */
struct keyspace {
    size_t num;
    char *key;
    size_t ks_sz;
    node_t *tail;
};

/*  Linked list node.  */
struct node
{
    size_t ver;
    info_t *info;
    node_t *next;
};

/*  Node payload.  */
struct info {
    size_t val;
    char *key;
};

