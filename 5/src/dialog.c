#include "dialog.h"
#include "generic.h"
#include "lib_graph.h"
#include <complex.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>



/*  DIALOG  */

void dialog (Dialog_info info, int (*fptr[]) (Graph, FILE*), Graph graph, FILE* file) {

    int fn_num = 0;
    
    puts (info->greeting);

    while ((fn_num = option_choice (info->msgs, info->msgc, file))) {
        if (fptr[fn_num](graph, file) == ERREOF)
            break;
    }

    puts (info->exit_msg);
}

int Import (Graph graph, FILE *file) {

    FILE *data = user_file();

    while (InsertVertex (graph, data) != ERREOF);

    fclose (data);

    return ERRSUC;
}



/*||||||||||||||||||||||||| < GENERAL FUNCTIONS > |||||||||||||||||||||||||*/


/*  INSERTION  */

int (*fptr_I[]) (Graph, FILE*)  = {NULL, InsertVertex, InsertEdge};


int Insert (Graph graph, FILE *file) {

    struct dialog_strings info = {NULL, NULL, 0, NULL};

    info.greeting = "\nInsert graph UI.";
    info.msgs = "\n0 - quit\n""1 - InsertVertex\n""2 - InsertEdge\n";
    info.msgc = 3;
    info.exit_msg = "quit Insert submenu";

    dialog (&info, fptr_I, graph, file);
        
    return ERRSUC;
}

int InsertVertex (Graph graph, FILE *file) {

    puts ("Enter computer name:");
    char *name = get_str (file);
    if (!name)
        return ERREOF;

    puts ("Enter service port:");
    size_t port = 0;
    if (get_sizet_file (file, &port, 255, 0) == ERREOF) {
        free_nullify (name);
        return ERREOF;
    }

    switch (add_vertex (graph, name, port))  {
        case ERRDUP:
            puts ("This name already exists");
            free_nullify (name);
            return ERRDUP;
        case ERROVF:
            puts ("Graph overflow");
            free_nullify (name);
            return ERROVF;
        default:
            puts ("Vertex inserted successfully.");
    }

    return ERRSUC;
}

int InsertEdge (Graph graph, FILE *file) {

    puts ("Enter first computer name");
    char *name1 = get_str (file);
    if (!name1)
        return ERREOF;
    
    puts ("Enter second computer name");
    char *name2 = get_str (file);
    if (!name2) {
        free_nullify (name1);
        return ERREOF;
    }

    puts ("Enter number of edge ports");
    size_t ports_num = 0;
    if (get_sizet_file (file, &ports_num, 255, 0) == ERREOF)
        return ERREOF;

    puts ("Enter edge ports");
    size_t *ports = calloc (ports_num, sizeof (size_t));
    for (size_t i = 0; i < ports_num; i++) {
        if (get_sizet_file (file, ports+i, 255, 0) == ERREOF)
            return ERREOF;
    }

    if (add_edge (graph, name1, name2, ports, ports_num) == ERRWRG) {
        puts ("Vertex not found");
        free_nullify (name1);
        free_nullify (name2);
        free_nullify (ports);
        return ERRWRG;
    } else 
        puts ("Edge inserted successfully.");

    free_nullify (name1);
    free_nullify (name2);

    return ERRSUC;
}


/*  DELETION  */

int (*fptr_D[]) (Graph, FILE*)  = {NULL, DeleteVertex, DeleteEdge};


int Delete (Graph graph, FILE *file) {

    struct dialog_strings info = {NULL, NULL, 0, NULL};

    info.greeting = "\nDelete graph UI.";
    info.msgs = "\n0 - quit\n""1 - DeleteVertex\n""2 - DeleteEdge\n";
    info.msgc = 3;
    info.exit_msg = "quit Delete submenu";

    dialog (&info, fptr_D, graph, file);
        
    return ERRSUC;
}

int DeleteVertex (Graph graph, FILE *file) {

    puts ("Enter computer name:");
    char *name = get_str (file);
    if (!name)
        return ERREOF;

    if (remove_vertex (graph, name) == ERRWRG)
        printf ("Vertex %s not found\n", name);
    else 
        puts ("Vertex deleted successfully.");

    free_nullify (name);

    return ERRSUC;
}

int DeleteEdge (Graph graph, FILE *file) {

    puts ("Enter first computer name");
    char *name1 = get_str (file);
    if (!name1)
        return ERREOF;

    puts ("Enter second computer name");
    char *name2 = get_str (file);
    if (!name2) {
        free_nullify (name1);
        return ERREOF;
    }

    if (remove_edge (graph, name1, name2) == ERRWRG) {
        puts ("Edge not found");
        free_nullify (name1);
        free_nullify (name2);
        return ERRWRG;
    }
    else 
        puts ("Vertex deleted successfully.");

    return ERRSUC;
}


/*  UPDATE  */

int (*fptr_U[]) (Graph, FILE*)  = {NULL, UpdateVertex, UpdateEdge};


int Update (Graph graph, FILE *file) {

    struct dialog_strings info = {NULL, NULL, 0, NULL};

    info.greeting = "\nUpdate graph UI.";
    info.msgs = "\n0 - quit\n""1 - UpdateVertex\n""2 - UpdateEdge\n";
    info.msgc = 3;
    info.exit_msg = "quit Update submenu";

    dialog (&info, fptr_U, graph, file);
        
    return ERRSUC;
}


int (*fptr_UV[]) (Graph, FILE*)  = {NULL, UpdateVertexName, UpdateVertexPort};

int UpdateVertex (Graph graph, FILE *file) {

    struct dialog_strings info = {NULL, NULL, 0, NULL};

    info.greeting = "\nUpdateVertex graph UI.";
    info.msgs = "\n0 - quit\n""1 - UpdateVertexName\n""2 - UpdateVertexPort\n";
    info.msgc = 3;
    info.exit_msg = "quit UpdateVertex submenu";

    dialog (&info, fptr_UV, graph, file);
        
    return ERRSUC;
}

int UpdateVertexName (Graph graph, FILE *file) {

    puts ("Enter old computer name:");
    char *old_name = get_str (file);
    if (!old_name)
        return ERREOF;

    Vertex v = find_vertex_in_graph (graph, old_name, NULL);

    if (!v) {
        printf ("Vertex %s not found\n", old_name);
        free_nullify (old_name);
        return ERRWRG;
    } 

    free_nullify (old_name);

    puts ("Enter new computer name:");
    char *new_name = get_str (file);
    if (!new_name)
        return ERREOF;

    if (find_vertex_in_graph (graph, new_name, NULL)) {
        puts ("This name already exists");
        free_nullify (new_name);
        return ERRDUP;
    }

    change_vertex_name (v->info, new_name);
    puts ("Vertex name updated successfully.");

    return ERRSUC;
}

int UpdateVertexPort (Graph graph, FILE *file) {

    puts ("Enter old computer name:");
    char *name = get_str (file);
    if (!name) 
        return ERREOF;

    Vertex v = find_vertex_in_graph (graph, name, NULL);

    if (!v) {
        printf ("Vertex %s not found\n", name);
        free_nullify (name);
        return ERRWRG;
    } 

    free_nullify (name);

    puts ("Enter new service port:");
    size_t new_port = 0;
    if (get_sizet_file (file, &new_port, 255, 0) == ERREOF)
        return ERREOF;

    change_vertex_port (v->info, new_port);
    puts ("Vertex port updated successfully.");

    return ERRSUC;
}


int (*fptr_UE[]) (Graph, FILE*)  = {NULL, UpdateEdgePorts};

int UpdateEdge (Graph graph, FILE *file) {

    struct dialog_strings info = {NULL, NULL, 0, NULL};

    info.greeting = "\nUpdateEdge graph UI.";
    info.msgs = "\n0 - quit\n""1 - UpdateEdgePorts\n";
    info.msgc = 2;
    info.exit_msg = "quit UpdateEdge submenu";

    dialog (&info, fptr_UE, graph, file);
        
    return ERRSUC;
}

int UpdateEdgePorts (Graph graph, FILE *file) {

    puts ("Enter first computer name");
    char *name1 = get_str (file);
    if (!name1)
        return ERREOF;

    puts ("Enter second computer name");
    char *name2 = get_str (file);
    if (!name2) {
        free_nullify (name1);
        return ERREOF;
    }

    puts ("Enter new number of edge ports");
    size_t ports_num = 0;
    if (get_sizet_file (file, &ports_num, 255, 0) == ERREOF)
        return ERREOF;

    puts ("Enter new edge ports");
    size_t *ports = calloc (ports_num, sizeof (size_t));
    for (size_t i = 0; i < ports_num; i++) {
        if (get_sizet_file (file, ports+i, 255, 0) == ERREOF)
            return ERREOF;
    }

    change_edge_ports (graph, name1, name2, ports);
    puts ("Edge ports updated successfully.");

    return ERRSUC;
}


/*  PRINT  */

int Print (Graph graph, FILE *file) {

    print_graph (graph);
    return ERRSUC;
}


/*  DFS  */

int DFS (Graph graph, FILE *file) {

    dfs (graph);

    return ERRSUC;
}