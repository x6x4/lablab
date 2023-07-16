#include "lib_graph.h"
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