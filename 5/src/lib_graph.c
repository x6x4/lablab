#include "lib_graph.h"
#include "generic.h"
#include "lib_bin_heap.h"
#include "lib_ll.h"
#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>



void print_graph (const Graph graph) {

    if (graph->sz == 0)
        printf ("(void)\n");
    
    for (size_t i = 0; i < graph->sz; i++) {
        V_header head = &(graph->adj_list[i]);

        if (head) {
            print_vertex_header (head);
            print_ll (head->head);
        }
        printf ("\n");
    }

}

void print_graph_comps (const Graph graph) {
    
    if (graph->sz == 0)
        printf ("(void)\n");
    
    for (size_t i = 0; i < graph->sz; i++) {
        V_header head = &(graph->adj_list[i]);

        if (head) {
            print_vertex_header_no_color (head);
            print_ll (head->head);
        }
        printf ("\n");
    }
}

void print_vertex_header (const V_header v) {

    if (v)
        printf (YELLOW("(%s, %lu) -> "), v->info->name, v->info->port);
}

void print_vertex_header_no_color (const V_header v) {

    if (v)
        printf ("(%s, %lu) -> ", v->info->name, v->info->port);
}

void print_vertex_header_no_color_rev (const V_header v) {

    if (v)
        printf ("(%s, %lu) <- ", v->info->name, v->info->port);
}

void free_graph (Graph graph) {

    for (size_t i = 0; i < graph->sz; i++) {
        V_header head = &(graph->adj_list[i]);
        free_ll (&(head->head), head->info->name);
        free_vertex_header (head);
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

    for (size_t i = 0; i < graph->sz; i++) 
        //  this covers all infos
        graph->adj_list[i].info->color_dfs = WHITE;
}

size_t handle_components (Graph g) {

    size_t comp_num = 0;

    for (size_t i = 0; i < g->sz; i++) {
        
        if (g->adj_list[i].info->color_dfs == WHITE) {

            visit_vertex_comps (g, &(g->adj_list[i]), comp_num);
            comp_num++;
        }
    }

    printf ("\x1b[0m");

    return comp_num;
}

void visit_vertex_comps (Graph g, V_header v, size_t comp_num) {

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
    
        V_header header = take_header_from_node (g, node);

        if (!header) return;
        
        /*  it is white  */
        if (header->info->color_dfs == WHITE) 
            visit_vertex_comps (g, header, comp_num);

        /*  otherwise keep looking  */
        node = node->next;
    }

    return;
}

Graph split_graph (Graph g, size_t comp_num) {

    Graph comps = calloc (comp_num, sizeof *comps);
    
    for (size_t i = 0; i < g->sz; i++) {

        size_t num = g->adj_list[i].comp_num;
        Graph cur_comp = comps+num;
        
        cur_comp->adj_list[cur_comp->sz] = g->adj_list[i];
        cur_comp->sz++;
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

V_header take_header_from_node (Graph g, V_node v) {

    if (!v) return NULL;

    V_header header = find_vertex_in_graph (g, v->info->name);

    return header;
}

#define INIT_CLR 1

size_t clr = INIT_CLR;

Bool branch_end_reached = 0;

void dfs (Graph g, V_header v, size_t port) {

    if (!v) return;

    init_colors (g);
    
    clr = INIT_CLR;

    print_dfs_paths (v, g, v, port);

    if (clr == INIT_CLR)
        puts ("No way");

    //  restore color
    printf ("\x1b[0m");
}


void print_dfs_paths (V_header start, Graph g, V_header v, size_t port) {

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
            print_vertex_header_no_color_rev (v);
            printf ("\n");

            clr++;
        }

        return;
    }

    while (node) {

        V_header node_h = take_header_from_node (g, node);

        if (!node_h) return;

        int is_connected = is_port_avail (node->edge_info->avail_ports, 
                            node->edge_info->ports_num, port);
    
        if (node_h->info->color_dfs == WHITE && is_connected) {

            print_dfs_paths (start, g, node_h, port);
            
            //  branch start reached
            if (branch_end_reached) {
            
                if (v == start) {

                    print_vertex_header_no_color_rev (start);

                    //  start new branch with new color
                    printf ("\n");
                    clr++;
                } 
                else 
                    break;
            } 
            
        } 
        else 
            /*  keep looking  */
            node = node->next;            
    }

    //  print nodes from the branch end 
    if (v != start) {

        //  valid branch end
        if (!branch_end_reached && v->info->port == port) {

            branch_end_reached = 1;
            printf ("\x1b[3%lum", clr%7);
            print_vertex_header_no_color_rev (v);
            
            return;
        } 

        //  intermediate node
        else if (branch_end_reached) {

            printf ("\x1b[3%lum", clr%7);
            print_vertex_header_no_color_rev (v);
        }
    } 

    return;
}   

Bool is_port_avail (size_t *ports, size_t ports_num, size_t port) {

    for (size_t i = 0; i < ports_num; i++) 
        if (ports[i] == port) return 1;

    return 0;
}



int djkstra (Graph g, char *name1, char *name2, size_t port) {

    V_header vh1 = check_vertices (g, name1, name2);

    if (!vh1)    
        return ERRWRG;

    V_header vh2 = find_vertex_in_graph (g, name2);

    if (vh2->info->port != port) {

        printf ("\nNo path\n");

        return ERRSUC;
    }

    size_t *path = NULL;

    size_t path_len = short_path (g, vh1, vh2, port, &path);

    if (path_len != INF) {
        printf ("\nPath length: %lu\n", path_len);
        
        for (size_t i = 0; i < path_len + 1; i++)
            printf ("%s ",  g->adj_list[path[i]].info->name);
        printf ("\n");
    }
    else 
        printf ("\nNo path\n");

    free_nullify (path);

    return ERRSUC;
}

size_t short_path (Graph g, V_header start, V_header end, size_t port, size_t **path) {

    //  init
    
    size_t *dist_from_start = calloc (g->sz, sizeof *dist_from_start);
    assert (dist_from_start);

    Bool *visited = calloc (g->sz, sizeof *visited);
    assert (visited);

    size_t *prev = calloc (g->sz, sizeof *prev);
    assert (dist_from_start);


    for (size_t i = 0; i < g->sz; i++) 
        dist_from_start[i] = prev[i] = INF;

    size_t start_v = num(start);
    size_t end_v = num(end);
    dist_from_start[start_v] = 0;
    visited[start_v] = 0;

    BHeap scanned = {};
    HEntry pair = {0, start_v};
    heap_insert (&scanned, &pair);


    //  main loop: reduce path length on every BFS step and go forth
    while (scanned.sz) {

        //  init

        heap_extract_top (&scanned, &pair);
        if (visited[pair.v])
            continue;

        size_t v_to_visit = pair.v;
        visited[v_to_visit] = 1;

        if (v_to_visit == end_v)
            break;

        V_node node_to_scan = g->adj_list[v_to_visit].head;

        //  scan adj list of current vertex
        while (node_to_scan) {

            //  init
            V_header neigh_to_scan = take_header_from_node (g, node_to_scan);
            size_t neigh_v = num(neigh_to_scan);
            if (!neigh_to_scan) break;

            int is_connected = is_port_avail (node_to_scan->edge_info->avail_ports, 
                            node_to_scan->edge_info->ports_num, port);
            size_t edge_weight = 1;
            size_t new_path_weight = pair.dist + edge_weight;

            //  neighbour relaxation
            {
                if (is_connected && new_path_weight < dist_from_start[neigh_v]) {

                    //  set neighbour dist  
                    dist_from_start[neigh_v] = new_path_weight;

                    //  set neighbour vertex
                    pair.v = neigh_v;
                    
                    //  set prev for neighbour (to cur vertex)
                    prev[pair.v] = v_to_visit;
                    
                    //  put neighbour to scanned
                    heap_insert (&scanned, &pair);
                }
            }

            /*  scan further  */
            node_to_scan = node_to_scan->next;
        }
    }


    //  fill path
    size_t path_len = dist_from_start[end_v];
    {
        if (path && path_len != INF) {

            *path = calloc (path_len + 1, sizeof *path);
            size_t prev_v = num(end);

            for (size_t i = path_len; prev_v != INF; i--) {
                (*path)[i] = prev_v;
                prev_v = prev[prev_v];
            }
        }
    }

    //  finit
    {
        heap_free (&scanned);
        free_nullify (visited);
        free_nullify (dist_from_start);
        free_nullify (prev);
    }

    return path_len;
}



int add_vertex (Graph graph, char *name, size_t port) {
    
    if (graph->sz == MAX_VERTEX_NUM)
        return ERROVF;

    if (find_vertex_in_graph (graph, name))
        return ERRDUP;

    graph->adj_list[graph->sz++].info = new_info (name, port);

    return ERRSUC;
}

V_node new_vertex_node (V_info info) {

    V_node v = calloc (1, sizeof *v);
    v->info = info;
    v->next = NULL;
    v->edge_info = NULL;
    
    return v;
}

V_info new_info (char *name, size_t port) {

    V_info info = calloc (1, sizeof *(info));
    info->name = name;
    info->port = port;
    info->color_dfs = WHITE;

    return info;
}

int remove_vertex (Graph g, char *name) {
    
    V_header v = find_vertex_in_graph (g, name);
    if (!v)
        return ERRWRG;

    size_t num = num(v);

    V_header dest = g->adj_list + num;
    V_header src = g->adj_list + num + 1;

    V_header header = &(g->adj_list[num]);

    //  free vertex adj list and edges infos
    free_ll (&(header->head), header->info->name);

    //  free matching vertex node in other lists and all in edges
    remove_vertex_from_adj_lists (g, name);
    
    //  free name and info
    free_vertex_header (header);

    //  shift to fill freed place
    memcpy (dest, src, (g->sz - num) * sizeof (*dest));

    g->sz--;

    return ERRSUC;
}

void free_vertex_header (V_header v) {

    if (v) {
        if (v->info) free_nullify (v->info->name);
        free_nullify (v->info);
    }
}

void remove_vertex_from_adj_lists (Graph graph, char *name) {

    Edge e = NULL;

    for (size_t i = 0; i < graph->sz; i++) {
        delete_from_ll (&(graph->adj_list[i].head), name, &e);
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


V_header find_vertex_in_graph (Graph graph, char *name) {

    if (graph->sz == 0)
        return NULL;

    for (size_t i = 0; i < graph->sz; i++) {

        if (graph->adj_list[i].info) {
            if (EQ(graph->adj_list[i].info->name, name)) 
                return &(graph->adj_list[i]);
        }
    }

    return NULL;
}



int add_edge (Graph g, char *name1, char *name2, size_t *avail_ports, size_t ports_num) {

    V_header vh1 = check_vertices (g, name1, name2);

    if (!vh1)
        return ERRWRG;

    V_node v1 = new_vertex_node (vh1->info);
    V_node v2 = NULL;
        
    Edge e = new_edge (avail_ports, ports_num);
    v1->edge_info = e;
    
    
    if (!EQ(name1, name2)) {

        V_header vh2 = find_vertex_in_graph (g, name2);
        v2 = new_vertex_node (vh2->info);
        v2->edge_info = e;
        insert_to_ll (&(vh2->head), v1, NULL);

    } else {
        v2 = v1;
    }

    insert_to_ll (&(vh1->head), v2, NULL);

    return ERRSUC;
}

V_header check_vertices (Graph graph, char *name1, char *name2) { 
    
    V_header v1 = find_vertex_in_graph (graph, name1);
    if (!v1) 
        return NULL;

    V_header v2 = find_vertex_in_graph (graph, name2);
    if (!v2) 
        return NULL;

    return v1;
}

int check_edge (Graph graph, char *name1, char *name2) {
    
    V_header v1 = find_vertex_in_graph (graph, name1);
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

    V_header vh1 = find_vertex_in_graph (graph, name1);
    V_header vh2 = find_vertex_in_graph (graph, name2);

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

    V_header vh1 = check_vertices (graph, name1, name2);

    if (!vh1)
        return ERRWRG;

    V_node v2 = find_in_ll (vh1->head, name2, NULL);

    free_nullify (v2->edge_info->avail_ports);

    v2->edge_info->avail_ports = new_avail_ports;
    v2->edge_info->ports_num = new_ports_num;

    return ERRSUC;
}

