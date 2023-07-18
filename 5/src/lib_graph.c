#include "lib_graph.h"
#include "generic.h"
#include "lib_ll.h"
#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>



void print_graph (Graph graph) {
    if (graph->csize == 0)
        printf ("(void)\n");
    
    for (size_t i = 0; i < graph->csize; i++) {
        print_ll (graph->adj_list[i]);
        printf ("\n");
    }

}

void free_graph (Graph graph) {

    for (size_t i = 0; i < graph->csize; i++)
        free_ll (&(graph->adj_list[i]));
}

size_t time;

void dfs (Graph graph) {

    init_dfs (graph);

    handle_dfs (graph);
}

void init_dfs (Graph graph) {

    for (size_t i = 0; i < graph->csize; i++) 
        graph->adj_list[i]->info->color_dfs = WHITE;
}

void handle_dfs (Graph graph) {

    for (size_t i = 0; i < graph->csize; i++) {
        if (graph->adj_list[i]->info->color_dfs == WHITE)
            visit_dfs (graph->adj_list[i]);
    }
}

void visit_dfs (Vertex v) {

    Vertex node = v;

    while (node) {

        print_node (node, v);

        if (node->info->color_dfs == WHITE) 
            node->info->color_dfs = GREY;

        node = node->next;
    }

    v->info->color_dfs = BLACK;
    
    printf ("\n");
}

int add_vertex (Graph graph, char *name, size_t port) {
    
    if (graph->csize == MAX_VERTEX_NUM)
        return ERROVF;

    size_t num = 0;
    if (find_vertex_in_graph (graph, name, &num))
        return ERRDUP;


    Vertex v = new_vertex (new_info(name, port));
    insert_to_ll (&(graph->adj_list[graph->csize]), v, NULL);

    graph->csize++;

    return ERRSUC;
}

Vertex new_vertex (V_info info) {

    Vertex v = calloc (1, sizeof *v);
    v->info = info;
    v->next = NULL;
    
    return v;
}

V_info new_info (char *name, size_t port) {
    V_info info = calloc (1, sizeof *(info));
    info->name = name;
    info->port = port;

    return info;
}

int remove_vertex (Graph graph, char *name) {
    size_t num = 0;
    Vertex v = find_vertex_in_graph (graph, name, &num);
    if (!v)
        return ERRWRG;

    Vertex *dest = graph->adj_list + num;
    Vertex *src = graph->adj_list + num + 1;

    free_ll (&(graph->adj_list[num]));

    memcpy (dest, src, (graph->csize - num) * sizeof (Vertex));

    remove_vertex_from_adj_lists (graph, name);
 
    graph->csize--;

    return ERRSUC;
}

void remove_vertex_from_adj_lists (Graph graph, char *name) {
    for (size_t i = 0; i < graph->csize; i++) {
        if ( !EQ(graph->adj_list[i]->info->name, name) )
            delete_from_ll (&(graph->adj_list[i]), name);
    }
}


void change_vertex_name (V_info v, char *new_name) {
    free_nullify (v->name);
    v->name = new_name;
}

void change_vertex_port (V_info v, size_t new_port) {
    v->port = new_port;
}


Vertex find_vertex_in_graph (Graph graph, char *name, size_t *num) {

    if (graph->csize == 0)
        return NULL;

    for (size_t i = 0; i < graph->csize; i++) {

        if (EQ(graph->adj_list[i]->info->name, name)) {

            if (num) *num = i;

            return graph->adj_list[i];
        }
    }

    return NULL;
}



int add_edge (Graph graph, char *name1, char *name2, size_t *avl_ports, size_t ports_num) {
    
    size_t num1 = 0, num2 = 0;
    if (check_vertexes (graph, name1, name2, &num1, &num2) == ERRWRG)
        return ERRWRG;

    Vertex v1 = graph->adj_list[num1];
    Vertex v2 = graph->adj_list[num2];

    v1 = new_vertex (v1->info);
    v2 = new_vertex (v2->info);

    Edge e = new_edge (avl_ports, ports_num);
    v1->weight = e;
    v2->weight = e;
    
    insert_to_ll (&(graph->adj_list[num1]), v2, NULL);
    if (!EQ(name1, name2))
        insert_to_ll (&(graph->adj_list[num2]), v1, NULL);

    return ERRSUC;
}

int check_vertexes (Graph graph, char *name1, char *name2, size_t *num1, size_t *num2) { 
    
    Vertex v1 = find_vertex_in_graph (graph, name1, num1);
    if (!v1) 
        return ERRWRG;

    Vertex v2 = find_vertex_in_graph (graph, name2, num2);
    if (!v2) 
        return ERRWRG;

    return ERRSUC;
}


Edge new_edge (size_t *avl_ports, size_t ports_num) {
    Edge e = calloc (1, sizeof *e);
    e->avl_ports = avl_ports;
    e->ports_num = ports_num;

    return e;
}


int remove_edge (Graph graph, char *name1, char *name2) {

    size_t num1 = 0, num2 = 0;
    if (check_vertexes (graph, name1, name2, &num1, &num2) == ERRWRG)
        return ERRWRG;

    Vertex v1 = graph->adj_list[num1];
    Vertex v2 = graph->adj_list[num2];

    remove_vertex_from_adj_lists (graph, name1);
    remove_vertex_from_adj_lists (graph, name2);

    return ERRSUC;
}

int change_edge_ports (Graph graph, char *name1, char *name2, size_t *new_avl_ports) {

    size_t num1 = 0, num2 = 0;
    if (check_vertexes (graph, name1, name2, &num1, &num2) == ERRWRG)
        return ERRWRG;

    Vertex v1 = graph->adj_list[num1];
    Vertex v2 = graph->adj_list[num2];

    v2 = find_in_ll (v1, name2, NULL);

    free_nullify (v2->weight->avl_ports);
    v2->weight->avl_ports = new_avl_ports;

    return ERRSUC;
}

