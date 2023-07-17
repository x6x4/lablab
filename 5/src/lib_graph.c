#include "lib_graph.h"
#include "generic.h"
#include "lib_ll.h"
#include <stddef.h>
#include <stdio.h>



/*||||||||||||||||||||||||| < GRAPH FUNCTIONS > |||||||||||||||||||||||||*/

void print_graph (Graph graph) {
    if (graph->csize == 0)
        printf ("(void)\n");
    
    for (size_t i = 0; i < MAX_VERTEX_NUM; i++)
        print_ll (graph->adj_list[i]);
}

/*||||||||||||||||||||||||| < VERTEX FUNCTIONS > |||||||||||||||||||||||||*/

int add_vertex (Graph graph, char *name, size_t port) {
    return ERRSUC;
}

void remove_vertex (Graph graph, Vertex v) {

}

int change_vertex_name (V_info v, char *new_name) {
    return ERRSUC;
}

void change_vertex_port (V_info v, size_t new_port) {

}


Vertex find_vertex_in_graph (Graph graph, char *name) {

    if (graph->csize == 0)
        return NULL;

    for (size_t i = 0; i < MAX_VERTEX_NUM; i++) {
        if (EQ(graph->adj_list[i]->info->name, name))
            return graph->adj_list[i];
    }

    return NULL;
}

/*||||||||||||||||||||||||| < EDGE FUNCTIONS > |||||||||||||||||||||||||*/


void add_edge (Vertex v1, Vertex v2, size_t *avl_ports) {
    
}

int remove_edge (Vertex v1, Vertex v2) {
    return ERRSUC;
}

void change_edge_ports (Vertex v1, Vertex v2, size_t *new_avl_ports) {

}