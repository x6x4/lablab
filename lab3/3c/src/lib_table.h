#include <stdlib.h>

typedef struct table table_t;
typedef struct keyspace ks_t;
typedef struct info info_t;

/*  Main structure.  */
struct table {
    ks_t **hsvec;
    size_t hsvec_max_sz;
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
    //  todo: delete
    char *key;
};

