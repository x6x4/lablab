#include <stdlib.h>

enum {
	ERREOF = -1,
	ERRSUC,
	ERRWRG
};

#define ks_offset(i) BEGIN+TABLE+KS*i

#define BEGIN 0
#define TABLE 40
#define KS 64
#define NODE 12
#define INFO 56

typedef struct table table_t;
typedef struct keyspace ks_t;
typedef struct ks_d ks_d;
typedef struct info info_t;
typedef struct node node_t;

typedef size_t offset_t;

struct ks_d {
    size_t num;
    size_t ks_sz;
    offset_t tail;
    offset_t key;
};

/*  Main structure.  */
struct table {
    size_t max_sz;
    size_t sz;
    size_t kslist_max_sz;
    size_t kslist_sz;
    offset_t kslist;
};

/*  Linked list of items with same key.  */
struct keyspace {
    size_t num;
    offset_t key;
    size_t ks_sz;
    offset_t tail;
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

