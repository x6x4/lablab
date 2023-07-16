#include <stdlib.h>

typedef struct vertex Vertex;

struct vertex {
    char *node_name;
    size_t port_num;
};

typedef struct vertex_node Vnode;

struct vertex_node {
    Vertex *info;
    Vnode *next;
};

typedef struct edge Edge;

struct edge {
    size_t *avl_ports;
};

typedef struct graph Graph;

#define MAX_VERTEX_NUM 10

struct graph {
    Vertex *adj_list [MAX_VERTEX_NUM];
};


