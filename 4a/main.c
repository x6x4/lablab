#include "src/dialog.h"

#include <stdio.h>
#include <limits.h>
#include <stdlib.h>



/*  GLOBAL VARIABLES  */

int (*fptr[]) (BstNodePtr*, FILE*)  = {NULL, insert_tree, delete_tree, 
                                      find_tree, findmin_tree, traverse_tree,
                                      import_tree, print_tree, timing_tree, dump_tree};


int main () {

    BstNodePtr root = NULL;
    FILE *file = stdin;
    struct dialog_strings info = {NULL, NULL, 0, NULL};

    info.greeting = "Binary search tree UI.";
    info.msgs = "\n0 - quit\n""1 - insert\n""2 - delete by key\n" \
                 "3 - find by key\n""4 - find min\n""5 - traverse\n" \
                 "6 - import\n""7 - print\n""8 - timing tree\n""9 - dump tree\n";

    info.msgc = 10;
    info.exit_msg = "quit";

    dialog (&info, fptr, &root, file);

    fclose (file);
    free_bst (&root);
    return 0;
}





