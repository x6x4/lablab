#include "lib_graph.h"
#include "generic.h"
#include "lib_ll.h"
#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include "binheap.h"


void print_graph (const Graph graph) {

    if (graph->csize == 0)
        printf ("(void)\n");
    
    for (size_t i = 0; i < graph->csize; i++) {
        V_head head = graph->adj_list[i];

        if (head) {
            print_vertex_head (head);
            print_ll (head->head);
        }
        printf ("\n");
    }

}

void print_graph_comps (const Graph graph) {
    
    if (graph->csize == 0)
        printf ("(void)\n");
    
    for (size_t i = 0; i < graph->csize; i++) {
        V_head head = graph->adj_list[i];

        if (head) {
            print_vertex_head_no_color (head);
            print_ll (head->head);
        }
        printf ("\n");
    }
}

void print_vertex_head (const V_head v) {

    if (v)
        printf (YELLOW("(%s, %lu) -> "), v->info->name, v->info->port);
}

void print_vertex_head_no_color (const V_head v) {

    if (v)
        printf ("(%s, %lu) -> ", v->info->name, v->info->port);
}

void free_graph (Graph graph) {

    for (size_t i = 0; i < graph->csize; i++) {
        V_head *head = &(graph->adj_list[i]);
        free_ll (&((*head)->head), (*head)->info->name);
        free_vertex_head (head);
    }
}



void create_components (Graph g) {

    if (!g) return;

    init_colors (g);

    size_t comp_num = handle_components (g);

    Graph comps = split_graph (g, comp_num);

    print_components (comps, comp_num);

    free_nullify (comps);
}

void init_colors (Graph graph) {

    for (size_t i = 0; i < graph->csize; i++) 
        //  this covers all infos
        graph->adj_list[i]->info->color_dfs = WHITE;
}

size_t handle_components (Graph g) {

    size_t comp_num = 0;

    for (size_t i = 0; i < g->csize; i++) {
        
        if (g->adj_list[i]->info->color_dfs == WHITE) {

            visit_vertex_comps (g, g->adj_list[i], comp_num);
            comp_num++;
        }
    }

    printf ("\x1b[0m");

    return comp_num;
}

void visit_vertex_comps (Graph g, V_head v, size_t comp_num) {

    if (!v) return;

    /*  mark vertex  */
    if (v->info->color_dfs == WHITE) {

        //print_vertex_head_comps (v);
        v->info->color_dfs = GREY;
        v->comp_num = comp_num;
    }
    
    /*  go to next unmarked vertex  */
    V_node node = v->head;

    while (node) {
    
        V_head head = take_head_from_node (g, node);

        if (!head) return;
        
        /*  it is white  */
        if (head->info->color_dfs == WHITE) 
            visit_vertex_comps (g, head, comp_num);

        /*  otherwise keep looking  */
        node = node->next;
    }

    return;
}

Graph split_graph (Graph g, size_t comp_num) {

    Graph comps = calloc (comp_num, sizeof *comps);
    
    for (size_t i = 0; i < g->csize; i++) {

        size_t num = g->adj_list[i]->comp_num;
        Graph cur_comp = comps+num;
        
        cur_comp->adj_list[cur_comp->csize] = g->adj_list[i];
        cur_comp->csize++;
    }

    return comps;

}

void print_components (Graph comps, size_t comp_num) {

    for (size_t i = 0; i < comp_num; i++) {

        size_t color = (i+1)%7;

        printf ("\x1b[3%lum", color);

        print_graph_comps (comps+i);
        printf ("\n");
    }

    printf ("\x1b[0m");
}

V_head take_head_from_node (Graph g, V_node v) {

    if (!v) return NULL;

    V_head head = find_vertex_in_graph (g, v->info->name, NULL);

    return head;
}

size_t clr = 1;

void dfs (Graph g, V_head v, size_t port) {

    if (!v) return;

    init_colors (g);
    
    clr = 1;

    print_dfs_forest (v, g, v, port);

    //  restore color
    printf ("\x1b[0m");
}

int branch_end_reached = 0;

void print_dfs_forest (V_head start, Graph g, V_head v, size_t port) {

    branch_end_reached = 0;

    if (!v) return;

    /*  mark vertex  */
    if (v->info->color_dfs == WHITE) 
        v->info->color_dfs = GREY;

    else 
        //  looks like it's dead branch
        //  vertex already visited and it wasn't catched early.
        return;
    
    
    /*  go to next unmarked vertex  */
    V_node node = v->head;

    /*  isolated vertex, no edges  */
    if (!node) {

        if (v->info->port == port) {
        
            printf ("\x1b[3%lum", clr%7);
            print_vertex_head_no_color (v);
            printf ("\n");
        }

        return;
    }

    while (node) {

        V_head head = take_head_from_node (g, node);

        if (!head) return;

        int is_connection_avail = is_port_avail (node->weight->avail_ports, 
                            node->weight->ports_num, port);
    
        if (head->info->color_dfs == WHITE && is_connection_avail) {

            print_dfs_forest (start, g, head, port);
            
            //  branch start reached
            if (branch_end_reached && v == start) {

                print_vertex_head_no_color (start);

                //  start new branch with new color
                printf ("\n");
                clr++;
            }
            
        } else 
            /*  keep looking  */
            node = node->next;            
    }

    //  print nodes from the branch end 
    if (v != start) {

        //  valid branch end
        if (!branch_end_reached && v->info->port == port) {

            branch_end_reached = 1;
            printf ("\x1b[3%lum", clr%7);
            print_vertex_head_no_color (v);
            
            return;
        } 

        //  intermediate node
        else if (branch_end_reached) {

            printf ("\x1b[3%lum", clr%7);
            print_vertex_head_no_color (v);
        }
    }

    return;
}   

Bool is_port_avail (size_t *ports, size_t ports_num, size_t port) {

    for (size_t i = 0; i < ports_num; i++) 
        if (ports[i] == port) return 1;

    return 0;
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

V_node new_vertex_node (V_info info) {

    V_node v = calloc (1, sizeof *v);
    v->info = info;
    v->next = NULL;
    v->weight = NULL;
    
    return v;
}

V_info new_info (char *name, size_t port) {
    V_info info = calloc (1, sizeof *(info));
    info->name = name;
    info->port = port;
    info->color_dfs = WHITE;

    return info;
}

int remove_vertex (Graph graph, char *name) {
    size_t num = 0;
    V_head v = find_vertex_in_graph (graph, name, &num);
    if (!v)
        return ERRWRG;

    V_head *dest = graph->adj_list + num;
    V_head *src = graph->adj_list + num + 1;

    V_head *head = &(graph->adj_list[num]);

    //  free vertex adj list and edges infos
    free_ll (&((*head)->head), (*head)->info->name);

    //  free matching vertex node in other lists and all in edges
    remove_vertex_from_adj_lists (graph, name);
    
    //  free name and info
    free_vertex_head (head);

    //  shift to fill freed place
    memcpy (dest, src, (graph->csize - num) * sizeof (V_head));

    graph->csize--;

    return ERRSUC;
}

void free_vertex_head (V_head *v) {

    if (*v) {
        if ((*v)->info) free_nullify ((*v)->info->name);
        free_nullify ((*v)->info);
    }
    free_nullify (*v);
}

void remove_vertex_from_adj_lists (Graph graph, char *name) {

    Edge e = NULL;

    for (size_t i = 0; i < graph->csize; i++) {
        delete_from_ll (&(graph->adj_list[i]->head), name, &e);
        free_edge (&e);
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



int add_edge (Graph graph, char *name1, char *name2, size_t *avail_ports, size_t ports_num) {
    
    size_t num1 = 0, num2 = 0;
    if (check_vertices (graph, name1, name2, &num1, &num2) == ERRWRG)
        return ERRWRG;

    V_head vh1 = graph->adj_list[num1];
    V_node v1 = new_vertex_node (vh1->info);
    V_node v2 = NULL;
        
    Edge e = new_edge (avail_ports, ports_num);
    v1->weight = e;
    
    
    if (!EQ(name1, name2)) {

        V_head vh2 = graph->adj_list[num2];
        v2 = new_vertex_node (vh2->info);
        v2->weight = e;
        insert_to_ll (&(vh2->head), v1, NULL);
    } else {
        v2 = v1;
    }

    insert_to_ll (&(vh1->head), v2, NULL);

    return ERRSUC;
}

int check_vertices (Graph graph, char *name1, char *name2, size_t *num1, size_t *num2) { 
    
    V_head v1 = find_vertex_in_graph (graph, name1, num1);
    if (!v1) 
        return ERRWRG;

    V_head v2 = find_vertex_in_graph (graph, name2, num2);
    if (!v2) 
        return ERRWRG;

    return ERRSUC;
}

int check_edge (Graph graph, char *name1, char *name2) {
    
    V_head v1 = find_vertex_in_graph (graph, name1, NULL);
    if (!v1) 
        return ERRWRG;

    if (find_in_ll (v1->head, name2, NULL))
        return ERRSUC;
    else
        return ERRWRG;
}

Edge new_edge (size_t *avail_ports, size_t ports_num) {
    Edge e = calloc (1, sizeof *e);
    e->avail_ports = avail_ports;
    e->ports_num = ports_num;

    return e;
}



int remove_edge (Graph graph, char *name1, char *name2) {

    if (check_edge (graph, name1, name2) == ERRWRG)
        return ERRWRG;

    Edge e = NULL;

    V_head vh1 = find_vertex_in_graph (graph, name1, NULL);
    V_head vh2 = find_vertex_in_graph (graph, name2, NULL);

    delete_from_ll (&(vh1->head), name2, &e);
    delete_from_ll (&(vh2->head), name1, &e);

    free_edge (&e);

    return ERRSUC;
}

void free_edge (Edge *e) {

    if (*e) free_nullify ((*e)->avail_ports);
    free_nullify ((*e));
}

int change_edge_ports (Graph graph, char *name1, char *name2, size_t *new_avail_ports, size_t new_ports_num) {

    size_t num1 = 0, num2 = 0;
    if (check_vertices (graph, name1, name2, &num1, &num2) == ERRWRG)
        return ERRWRG;

    V_head vh1 = graph->adj_list[num1];

    V_node v2 = find_in_ll (vh1->head, name2, NULL);

    free_nullify (v2->weight->avail_ports);

    v2->weight->avail_ports = new_avail_ports;
    v2->weight->ports_num = new_ports_num;

    return ERRSUC;
}

size_t dijktra(const Graph g, V_head start, V_head fin, size_t port) {
    size_t* dist = calloc(g->csize, sizeof(size_t));
    Bool* used = calloc(g->csize, sizeof(Bool));
    assert(dist);
    for(size_t i = 0; i < g->csize; ++i) {
        dist[i] = VERT_UNREACHABLE;
        g->adj_list[i]->g_idx = i;
    }

    size_t vert = start->g_idx;

    dist[vert] = 0;
    used[vert] = 1;

    heap_elem_t pair = {0, vert};
    BinHeap heap = {};
    heap_insert(&heap, &pair);

    while(heap.size != 0) {
        heap_extract_top(&heap, &pair);
        if(used[pair.vert]) {
            continue;
        }
        used[pair.vert] = 1;
        if(pair.vert == fin->g_idx)
            break; // We done.
        
        pair.dist++; // For next verts.

        V_node node = g->adj_list[vert]->head;
        while (node) {

            V_head head = take_head_from_node (g, node);

            if (!head) break;

            int is_connection_avail = is_port_avail (node->weight->avail_ports, 
                                node->weight->ports_num, port);
        
            if (is_connection_avail && (size_t)pair.dist < dist[head->g_idx]) {
                dist[head->g_idx] = (size_t)pair.dist;
                pair.vert = head->g_idx;
                heap_insert(&heap, &pair);
            }
            /*  keep looking  */
            node = node->next;            
        }
    }

    size_t ans = dist[fin->g_idx];
    free_heap(&heap);
    free_nullify(used);
    free_nullify(dist);
    return ans;
}