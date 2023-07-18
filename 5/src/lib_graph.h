#pragma once

#include <stddef.h>
#include <stdlib.h>


typedef struct edge *Edge;

struct edge {

    size_t ports_num;
    size_t *avl_ports;
};

typedef struct v_info *V_info;

struct v_info {

    char *name;
    size_t port;

    int color_dfs;
};

typedef struct vertex *Vertex;

struct vertex {

    V_info info;
    Vertex next;
    Edge weight;

    
};

enum {
    WHITE, 
    GREY,
    BLACK
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
* @param graph [IN] - graph for addition. 
* @param name  [IN] - UNIQUE computer name. 
* @param port  [IN] - port number on which network service is running.
* @return int - Error code. Possible ERRDUP - computer name already exists.
  ERROVF - vertex limit is reached. 
*/
int add_vertex (Graph graph, char *name, size_t port);

/**
* @brief Create new isolated vertex. 
* 
* @param info [IN] - vertex info
* @return Vertex - ptr to vertex created.
*/
Vertex new_vertex (V_info info);

/**
* @brief Check if both vertex belong to graph. 
* 
* @param graph [IN] - graph for check. 
* @param name1, name2  [IN] - UNIQUE vertex names. 
* @param num1, num2   [IN/OUT] OPTIONAL - numbers of vertexes in adjacency lists
* @return int - Error code. Possible ERRWRG - one or two names not found.
*/
int check_vertexes (Graph graph, char *name1, char *name2, size_t *num1, size_t *num2);

/**
* @brief Create new isolated vertex info. 
* 
* @param name [IN] - UNIQUE computer name. 
* @param port [IN] - port number on which network service is running.
* @return V_info - ptr to info created.
*/
V_info new_info (char *name, size_t port);

/**
* @brief Remove vertex from graph. 
* 
* @param graph [IN] - graph for removal. 
* @param name  [IN] - UNIQUE computer name. 
* @return int - Error code. Possible ERRWRG - name not found.
*/
int remove_vertex (Graph graph, char *name);

/**
* @brief Remove vertex from adjacency lists (BESIDES its own). 
* 
* @param graph [IN] - graph for removal. 
* @param name  [IN] - UNIQUE computer name. 
* @return int - Error code. Possible ERRWRG - name not found.
*/
void remove_vertex_from_adj_lists (Graph graph, char *name);

/**
* @brief Change name field of vertex. 
* 
* @param v        [IN] - vertex which name is changed.
* @param new_name [IN] - new UNIQUE computer name. 
*/
void change_vertex_name (V_info v, char *new_name);

/**
* @brief Change port field of vertex. 
* 
* @param v        [IN] - vertex which name is changed.
* @param new_port [IN] - new port number on which network service is running.
*/
void change_vertex_port (V_info v, size_t new_port);

/**
* @brief Find vertex by name. 
* 
* @param graph [IN] - graph for search
* @param name  [IN] - UNIQUE computer name
* @param num   [IN/OUT] OPTIONAL - number in adjacency list
* @return Vertex - head of matching adjacency list. NULL if not found
*/
Vertex find_vertex_in_graph (Graph graph, char *name, size_t *num);



/*||||||||||||||||||||||||| < EDGE FUNCTIONS > |||||||||||||||||||||||||*/

/*  Note: loops and multiple edges are allowed. */

/**
* @brief Add edge to graph. 
* 
* @param graph [IN] - graph for addition. 
* @param name1, name2   [IN] - names of ends of newly created edge. 
* @param avl_ports [IN] - vector of ports, available for traffic transmission.
* @param ports_num [IN] - number of ports.
* @return int - Error code. Possible ERRWRG - vertex not found.
*/
int add_edge (Graph graph, char *name1, char *name2, size_t *avl_ports, size_t ports_num);

/**
* @brief Create new isolated edge. 
* 
* @param avl_ports [IN] - vector of ports, available for traffic transmission.
* @param ports_num [IN] - number of ports.
* @return Edge - ptr to edge created.
*/
Edge new_edge (size_t *avl_ports, size_t ports_num);

/**
* @brief Remove edge from graph. 
* 
* @param graph [IN] - graph for removal. 
* @param name1, name2   [IN] - names of ends of removed edge. 
* @return int - Error code. Possible ERRWRG - edge not found.
*/
int remove_edge (Graph graph, char *name1, char *name2);

/**
* @brief Change avl_ports field of edge. 
* 
* @param graph [IN] - graph for change. 
* @param name1, name2   [IN] - names of ends of changed edge. 
* @param new_avl_ports [IN] - new vector of ports, available for traffic transmission.
*/
int change_edge_ports (Graph graph, char *name1, char *name2, size_t *new_avl_ports);



/*||||||||||||||||||||||||| < GRAPH FUNCTIONS > |||||||||||||||||||||||||*/

/**
* @brief Print graph as adjacency lists.
* 
* @param graph [IN] - graph to print.
*/
void print_graph (Graph graph);

/**
* @brief Clear graph.
* 
* @param graph [IN] - graph to clear.
*/
void free_graph (Graph graph);

/**
* @brief Depth-first search.
* 
* @param graph [IN] - graph for DFS.
*/
void dfs (Graph graph);

/**
* @brief Init graph before DFS.
  Set all vertices to white.
* 
* @param graph [IN] - graph for DFS.
*/
void init_dfs (Graph graph);

/**
* @brief Handle graph in DFS.
* 
* @param graph [IN] - graph for DFS.
*/
void handle_dfs (Graph graph);

/**
* @brief Visit vertex in DFS.
  Set vertice to grey, traverse its adjacency list 
  and set it to black. 
* 
* @param v[IN] - vertex to visit.
*/
void visit_dfs (Vertex v);