#include "dialog.h"
#include "generic.h"
#include "lib_graph.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>


/*  GLOBAL VARIABLES  */

int (*fptr[]) (Graph, FILE*)  = {NULL, Insert, Delete, Update, Print};


/*  DIALOG  */

void dialog (Dialog_info info, Graph graph, FILE* file) {

    int fn_num = 0;
    
    puts (info->greeting);

    while ((fn_num = option_choice (info->msgs, info->msgc, file))) {
        if (fptr[fn_num](graph, file) == ERREOF)
            break;
    }

    puts (info->exit_msg);
}


/*||||||||||||||||||||||||| < GENERAL FUNCTIONS > |||||||||||||||||||||||||*/


/*  INSERTION  */

const char *msgs_I [] = {};

int (*fptr_I[]) (Graph, FILE*)  = {NULL, InsertVertex, InsertEdge};

int Insert (Graph graph, FILE *file) {

    struct dialog_strings info_I = {NULL, NULL, 0, NULL};

    info_I.greeting = "\nInsert graph UI.";
    info_I.msgs = "\n0 - quit\n""1 - InsertVertex\n""2 - InsertEdge\n";
    info_I.msgc = 3;
    info_I.exit_msg = "quit Insert submenu";

    dialog (&info_I, graph, file);
        
    return ERRSUC;
}

int InsertVertex (Graph graph, FILE *file) {
    puts ("Enter computer name:");
    char *name = get_str (file);
    if (!name)
        return ERREOF;

    puts ("Enter service port:");
    size_t port = 0;
    if (get_sizet_file (file, &port, 255, 0) == ERREOF)
        return ERREOF;

    if (add_vertex (graph, name, port) == ERRDUP)
        puts ("This name already exists");
    else 
        puts ("Vertex inserted successfully.");

    return ERRSUC;
}

int InsertEdge (Graph graph, FILE *file) {
    puts ("Enter first computer name");
    char *name1 = get_str (file);
    if (!name1)
        return ERREOF;
    
    Vertex v1 = find_vertex_in_graph (graph, name1);

    puts ("Enter second computer name");
    char *name2 = get_str (file);
    if (!name2)
        return ERREOF;

    Vertex v2 = find_vertex_in_graph (graph, name2);

    puts ("Enter number of ports of edge");
    size_t ports_num = 0;
    if (get_sizet_file (file, &ports_num, 255, 0) == ERREOF)
        return ERREOF;

    size_t *ports = calloc (ports_num, sizeof (size_t));
    for (size_t i = 0; i < ports_num; i++) {
        if (get_sizet_file (file, ports+i, 255, 0) == ERREOF)
            return ERREOF;
    }

    add_edge (v1, v2, ports);
    puts ("Edge inserted successfully.");

    return ERRSUC;
}

/*  DELETION  */

int Delete (Graph graph, FILE *file) {
    return ERRSUC;
}

/*  UPDATE  */

int Update (Graph graph, FILE *file) {
    return ERRSUC;
}

/*  PRINT  */

int Print (Graph graph, FILE *file) {
    return ERRSUC;
}