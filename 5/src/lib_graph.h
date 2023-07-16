#pragma once

#include <stddef.h>
#include <stdlib.h>

typedef struct vertex Vertex;

struct vertex {
    char *name;
    size_t port;
};

typedef struct vertex_node *Vnode;

struct vertex_node {
    Vertex *info;
    Vnode next;
};

typedef struct edge Edge;

struct edge {
    size_t *avl_ports;
};

typedef struct graph *Graph;

#define MAX_VERTEX_NUM 10

struct graph {
    Vnode adj_list [MAX_VERTEX_NUM];
    size_t csize;
};

/*||||||||||||||||||||||||| < VERTEX FUNCTIONS > |||||||||||||||||||||||||*/

/**
* @brief Add vertex to graph. 
* 
* @param graph [IN]     - graph for insertion. 
* @param node_name [IN] - UNIQUE computer name. 
* @param port [IN]  - port number on which network service is running.
* @return int - Error code. Possible ERRDUP - computer name already exists.
*/
int add_vertex (Graph graph, char *name, size_t port);

/**
* @brief Delete vertex from graph. 
* 
* @param graph [IN]     - graph for deletion. 
* @param node_name [IN] - UNIQUE computer name. 
* @return int - Error code. Possible ERRWRG - computer name not found.
*/
int delete_vertex (Graph graph, char *name);

/**
* @brief Change name field of vertex. 
* 
* @param v [IN]     - vertex which name is changed.
* @param new_name [IN] - new UNIQUE computer name. 
* @return int - Error code. Possible ERRDUP - computer name already exists.
*/
int change_vertex_name (Vertex v, char *new_name);

/**
* @brief Change port field of vertex. 
* 
* @param v [IN]     - vertex which name is changed.
* @param new_port [IN] - new port number on which network service is running.
*/
void change_vertex_port (Vertex v, size_t new_port);



/*||||||||||||||||||||||||| < EDGE FUNCTIONS > |||||||||||||||||||||||||*/

/*  Note: loops and multiple edges are allowed. */

/**
* @brief Add edge to graph. 
* 
* @param v1, v2 [IN]     - ends of newly created edge. 
* @param avl_ports [IN]  - vector of ports, available for traffic transmission.
*/
void add_edge (Vertex v1, Vertex v2, size_t *avl_ports);

/**
* @brief Delete edge from graph. 
* 
* @param v1, v2 [IN]     - ends of deleted edge.
*/
void delete_edge (Vertex v1, Vertex v2);

/**
* @brief Change avl_ports field of edge. 
* 
* @param v [IN]     - vertex which name is changed.
* @param new_avl_ports [IN] - new vector of ports, available for traffic transmission.
* @return int - Error code. Possible ERRACC - edge ports don't match vertex ports.
*/
int change_edge_ports (Vertex v, size_t *new_avl_ports);



/*||||||||||||||||||||||||| < GRAPH FUNCTIONS > |||||||||||||||||||||||||*/

/**
* @brief Print graph as adjacency lists.
* 
* @param graph [IN] - graph to print.
*/
void print_graph (Graph graph);