#include "src/dialog.h"
#include "src/lib_graph.h"
#include <stdlib.h>
#include <string.h>



int main (void) {

    struct graph G = {{NULL}, 0};
    FILE *file = stdin;
    struct dialog_strings info = {NULL, NULL, 0, NULL};

    info.greeting = "Network graph UI.";
    info.msgs = "\n0 - quit\n""1 - Insert\n""2 - Delete\n" \
                 "3 - Update\n""4 - Print\n";
    info.msgc = 5;
    info.exit_msg = "quit";

    dialog (&info, &G, file);

    fclose (file);

    return 0;
}