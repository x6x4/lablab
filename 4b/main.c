#include "src/lib_bt.h"
#include "src/lib_bt_func.h"
#include "src/generic.h"
#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <limits.h>
#include <stdlib.h>

int insert_tree (BNodePtr *root, FILE *file);
int delete_tree (BNodePtr *root, FILE *file);
int find_tree (BNodePtr *root, FILE *file);
int findmax_tree (BNodePtr *root, FILE *file);
int traverse_tree (BNodePtr *root, FILE *file);
int import_tree (BNodePtr *root, FILE *file);
int print_tree (BNodePtr *root, FILE *file);
int dump_tree (BNodePtr *root, FILE *file);
void dump_node (BNodePtr root, FILE *file);

/*  GLOBAL VARIABLES  */

const char *msgs [] = {"\n0 - quit", "1 - insert", "2 - delete by key+version",
                       "3 - find by key", "4 - find max", "5 - traverse", 
                       "6 - import", "7 - print", "8 - dump"};
const size_t msgc = sizeof msgs / sizeof msgs[0];

int (*fptr[]) (BNodePtr*, FILE*)  = {NULL, insert_tree, delete_tree, 
                                      find_tree, findmax_tree, traverse_tree,
                                      import_tree, print_tree, dump_tree};

/*  MAIN  */

int main () {

    FILE *file = stdin;
    BNodePtr root = NULL;
    int fn_num = 0;
    printf ("B23 tree UI.\n");

    while ((fn_num = option_choice (msgs, msgc, file))) {
        if (fptr[fn_num](&root, file) == ERREOF)
            break;
    }

    fclose (file);
    free_bt (&root);
    puts ("quit");
    return 0;
}

static size_t dump_num = 0;
int dump_tree (BNodePtr *root, FILE *file) {

    char error_string[255] = {};
    char *dump_name = "tree_dump.dot";

    FILE *dump_file = fopen (dump_name, "w");
    if (!dump_file) perror (error_string);

    fprintf (dump_file, "strict graph {\nnode [shape=record];\n");
    dump_node (*root, dump_file);
    fprintf (dump_file, "}\n");

    fclose (dump_file);

    char cmd[255] = {};
    system ("mkdir -p dumps");
    sprintf (cmd, "dot %s -T png -o dumps/dump_%lu.png", dump_name, dump_num++);
    system(cmd);

    return ERRSUC;
}

void dump_node (BNodePtr node, FILE *file) {

    if (!node) {
        fprintf (file, "NULL\n");
        return;
    }

    fprintf (file, "node_%p [label=\"", node);
    for (size_t i = 0; i < node->csize; i++)
        fprintf (file, "%s%c", node->info[i]->head->val, i == node->csize - 1 ? '\"' : '|');
    fprintf (file, "];\n");

    if (!is_leaf(node)) {
        for (size_t i = 0; i < node->csize + 1; i++) {
            assert (node->child[i]);
            dump_node (node->child[i], file);
            fprintf (file, "node_%p -- node_%p\n", node, node->child[i]);
        }
    }

}

int insert_tree (BNodePtr *root, FILE *file) {
    
    char *key = NULL, *val = NULL;
    const char *s = "Enter key of item to insert: \n";

    printf ("%s", s);

    key = get_str (file);
    if (!key)
        return ERREOF;

    s = "Enter value of item to insert: \n";

    printf ("%s", s);

    val = get_str (file);
    if (!val)
        return ERREOF;   

    insert_bt (root, key, val);
    printf ("\nItem inserted successfully.\n");

    free_nullify (key);
    free_nullify (val);

    return ERRSUC; 
};

int delete_tree (BNodePtr *root, FILE *file) {

    char *key = NULL;
    size_t ver = 0;
    const char *s = "Enter key of item to delete: \n";

    printf ("%s", s);
     
    key = get_str (file);
    if (!key)
        return ERREOF;

    s = "Enter version of item to delete: \n";

    printf ("%s", s);
    
    int status = get_int_file (file, (int*) &ver, INT_MAX, 0);
    if (status != ERRSUC) 
        return status;  

    switch (delete_bt (root, key, ver)) {
        case ERRSUC:
            printf ("\nItem deleted successfully.\n");
            break;
        case ERRWRG:
            printf ("No such key or version.\n");
            break;
        default:
            printf (RED("Unknown error."));
    }

    print_tree (root, file);

    free (key);

    return ERRSUC; 
};

int find_tree (BNodePtr *root, FILE *file) {

    BNodePtr node = NULL;
    size_t pos = 0;

    char *key = NULL;
    const char *s = "Enter key of item to find: \n";

    printf ("%s", s);
    
    key = get_str (file);
    if (!key)
        return ERREOF;

    node = find_bt (*root, key, &pos);

    if (node) 
        colored_print_bt (*root, key);
    else 
        printf ("No such key\n");
    
    free (key);

    return ERRSUC;
};

int findmax_tree (BNodePtr *root, FILE *file) {

    BNodePtr node = find_max_node (*root);

    if (node) {
        size_t pos = node->csize - 1;
        colored_print_bt (*root, node->info[pos]->key);
    }
    else
        puts ("Void tree.");

    return ERRSUC;
};

int traverse_tree (BNodePtr *root, FILE *file) {

    const char *s = "Enter key to traverse from or \"a\" to traverse all tree: \n";

    printf ("%s", s);
    printf ("\nTraverse:\n");

    if (!(*root))
        printf ("(void)");
    else 
        traverse_bt (*root);

    printf ("\n");
    return ERRSUC;
}

int import_tree (BNodePtr *root, FILE *file) {

    FILE *import = user_file ("r");
    if (!import) 
        return ERREOF;

    while (insert_tree (root, import) != ERREOF);

    fclose (import);

    return ERRSUC;
}

int print_tree (BNodePtr *root, FILE *file) {
    
    printf ("\nTree:\n");
    if (!(*root) || (*root)->csize == 0) {
        printf ("(void)\n");
        return 1;
    }

    (*root)->height = 0;
    colored_print_bt (*root, NULL);
    printf ("\n");

    int status = -1;
    const char *s = "\nUnwrap some branch? (y/n): \n";
    while ((status = user_choice (s)) == ERRSUC) 
        find_tree (root, file);

    if (status == ERREOF)
        return ERREOF;
    else 
        return ERRSUC;
};




