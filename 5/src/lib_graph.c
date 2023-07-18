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
        V_head head = graph->adj_list[i];

        print_vertex_head (head);
        print_ll (head->head);
        printf ("\n");
    }

}

void print_vertex_head (V_head v) {

    if (v)
        printf (YELLOW("(%s, %lu) -> "), v->info->name, v->info->port);
}

void free_graph (Graph graph) {

    for (size_t i = 0; i < graph->csize; i++)
        free_ll (&(graph->adj_list[i]->head));
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
        if (graph->adj_list[i]->info->color_dfs == WHITE) {

            print_vertex_head (graph->adj_list[i]);
            visit_dfs (graph->adj_list[i]->head);
        }
    }
}

void visit_dfs (Vertex v) {

    if (!v) {
        printf ("\n");
        return;
    }

    Vertex node = v;

    while (node) {

        if (node->info->color_dfs == WHITE) {

            node->info->color_dfs = GREY;
            print_node (node, v);
        }

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


    V_head v = new_vertex_head (new_info(name, port));
    graph->adj_list[graph->csize++] = v;

    return ERRSUC;
}

V_head new_vertex_head (V_info info) {

    V_head v = calloc (1, sizeof *v);
    v->head = NULL;
    v->info = info;

    return v;
}

Vertex new_vertex (V_info info) {

    Vertex v = calloc (1, sizeof *v);
    v->info = info;
    v->next = NULL;
    v->weight = NULL;
    
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
    V_head v = find_vertex_in_graph (graph, name, &num);
    if (!v)
        return ERRWRG;

    V_head *dest = graph->adj_list + num;
    V_head *src = graph->adj_list + num + 1;

    free_ll (&(graph->adj_list[num]->head));

    memcpy (dest, src, (graph->csize - num) * sizeof (V_head));

    remove_vertex_from_adj_lists (graph, name);
 
    graph->csize--;

    return ERRSUC;
}

void remove_vertex_from_adj_lists (Graph graph, char *name) {
    for (size_t i = 0; i < graph->csize; i++) {
        if ( !EQ(graph->adj_list[i]->info->name, name) )
            delete_from_ll (&(graph->adj_list[i]->head), name);
    }
}


void change_vertex_name (V_info v, char *new_name) {
    free_nullify (v->name);
    v->name = new_name;
}

void change_vertex_port (V_info v, size_t new_port) {
    v->port = new_port;
}


V_head find_vertex_in_graph (Graph graph, char *name, size_t *num) {

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

    V_head vh1 = graph->adj_list[num1];
    V_head vh2 = graph->adj_list[num2];

    Vertex v1 = new_vertex (vh1->info);
    Vertex v2 = new_vertex (vh2->info);

    Edge e = new_edge (avl_ports, ports_num);
    v1->weight = e;
    v2->weight = e;
    
    insert_to_ll (&(vh1->head), v2, NULL);
    if (!EQ(name1, name2))
        insert_to_ll (&(vh2->head), v1, NULL);

    return ERRSUC;
}

int check_vertexes (Graph graph, char *name1, char *name2, size_t *num1, size_t *num2) { 
    
    V_head v1 = find_vertex_in_graph (graph, name1, num1);
    if (!v1) 
        return ERRWRG;

    V_head v2 = find_vertex_in_graph (graph, name2, num2);
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

    remove_vertex_from_adj_lists (graph, name1);
    remove_vertex_from_adj_lists (graph, name2);

    return ERRSUC;
}

int change_edge_ports (Graph graph, char *name1, char *name2, size_t *new_avl_ports) {

    size_t num1 = 0, num2 = 0;
    if (check_vertexes (graph, name1, name2, &num1, &num2) == ERRWRG)
        return ERRWRG;

    V_head vh1 = graph->adj_list[num1];
    V_head vh2 = graph->adj_list[num2];

    Vertex v2 = find_in_ll (vh1->head, name2, NULL);

    free_nullify (v2->weight->avl_ports);
    v2->weight->avl_ports = new_avl_ports;

    return ERRSUC;
}

