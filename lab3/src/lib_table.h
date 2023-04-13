#include <stdlib.h>

typedef struct table table_t;
typedef struct info info_t;
typedef struct node node_t;


struct table {
    node_t **nodes;
    size_t max_sz;
    size_t cur_sz;
};

struct node
{
    size_t ver;
    info_t *info;
    node_t *next;
};

struct info {
    size_t val;
    char *key;
};

