#pragma once

#include <stddef.h>
#include <stdlib.h>

typedef struct v_info *V_info;

struct v_info {
    char *name;
    size_t port;
};

typedef struct vertex *Vertex;

struct vertex {
    V_info info;
    Vertex next;
};

typedef struct edge Edge;

struct edge {
    size_t *avl_ports;
};

typedef struct graph *Graph;

#define MAX_VERTEX_NUM 10

struct graph {
    Vertex adj_list [MAX_VERTEX_NUM];
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
* @brief remove vertex from graph. 
* 
* @param graph [IN]     - graph for removal. 
* @param v [IN] - vertex to remove. 
*/
void remove_vertex (Graph graph, Vertex v);

/**
* @brief Change name field of vertex. 
* 
* @param v [IN]     - vertex which name is changed.
* @param new_name [IN] - new UNIQUE computer name. 
* @return int - Error code. Possible ERRDUP - computer name already exists.
*/
int change_vertex_name (V_info v, char *new_name);

/**
* @brief Change port field of vertex. 
* 
* @param v [IN]     - vertex which name is changed.
* @param new_port [IN] - new port number on which network service is running.
*/
void change_vertex_port (V_info v, size_t new_port);

Vertex find_vertex_in_graph (Graph graph, char *name);

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
* @brief Remove edge from graph. 
* 
* @param v1, v2 [IN]     - ends of removed edge.
* @return int - Error code. Possible ERRWRG - edge not found.
*/
int remove_edge (Vertex v1, Vertex v2);

/**
* @brief Change avl_ports field of edge. 
* 
* @param v1, v2 [IN]     - ends of newly created edge. 
* @param new_avl_ports [IN] - new vector of ports, available for traffic transmission.
*/
void change_edge_ports (Vertex v1, Vertex v2, size_t *new_avl_ports);



/*||||||||||||||||||||||||| < GRAPH FUNCTIONS > |||||||||||||||||||||||||*/

/**
* @brief Print graph as adjacency lists.
* 
* @param graph [IN] - graph to print.
*/
void print_graph (Graph graph);