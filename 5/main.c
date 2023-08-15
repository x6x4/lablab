#include "src/dialog.h"
#include "src/lib_graph.h"
#include <stdlib.h>

/*  GLOBAL VARIABLES  */

int (*fptr[]) (Graph, FILE*) = 
{NULL, Insert, Delete, Update, Print, Import, 
CreateComponents, DFS, Djkstra};


int main (void) {

    struct graph G = {{}, 0};
    FILE *file = stdin;
    struct dialog_strings info = {NULL, NULL, 0, NULL};

    info.greeting = "Network graph UI.";
    info.msgs = "\n0 - quit\n""1 - Insert\n""2 - Delete\n" \
                 "3 - Update\n""4 - Print\n""5 - Import\n" \
                 "6 - Split on components\n"               \
                 "7 - DFS\n""8 - Shortest path\n";

    info.msgc = 9;
    info.exit_msg = "quit";

    dialog (&info, fptr, &G, file);

    free_graph (&G);
    fclose (file);

    return 0;
}