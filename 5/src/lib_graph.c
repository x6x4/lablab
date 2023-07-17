#include "lib_graph.h"
#include "generic.h"
#include "lib_ll.h"
#include <stddef.h>
#include <stdio.h>



/*||||||||||||||||||||||||| < GRAPH FUNCTIONS > |||||||||||||||||||||||||*/

int add_vertex (Graph graph, char *name, size_t port) {
    return ERRSUC;
}

Vertex find_vertex_in_graph (Graph graph, char *name) {
    
    for (size_t i = 0; i < MAX_VERTEX_NUM; i++) {
        if (EQ(graph->adj_list[i]->info->name, name))
            return graph->adj_list[i];
    }

    return NULL;
}

void add_edge (Vertex v1, Vertex v2, size_t *avl_ports) {
    
}

void print_graph (Graph graph) {
    if (graph->csize == 0)
        printf ("(void)\n");
    
    for (size_t i = 0; i < MAX_VERTEX_NUM; i++)
        print_ll (graph->adj_list[i]);
}