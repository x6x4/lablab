#include <stdlib.h>

typedef struct table table_t;
typedef struct keyspace ks_t;
typedef struct info info_t;


/*  Main structure.  */
struct table {
    ks_t **hslist;
    size_t hslist_max_sz;
    size_t hslist_sz;
};

/*  Linked list node (same hash).  */
struct keyspace {
    char *key;
    info_t *info;
    ks_t *next;
};

/*  Node payload.  */
struct info {
    size_t val;
    char *key;
};

