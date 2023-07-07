#include "src/lib_bintree.h"
#include "src/generic.h"
#include <stddef.h>
#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <time.h>

#define eof 0

int insert_tree (BNodePtr *root, FILE *file);
int delete_tree (BNodePtr *root, FILE *file);
int find_tree (BNodePtr *root, FILE *file);
int findmax_tree (BNodePtr *root, FILE *file);
int traverse_tree (BNodePtr *root, FILE *file);
int import_tree (BNodePtr *root, FILE *file);
int print_tree (BNodePtr *root, FILE *file);
int timing_tree (BNodePtr *root, FILE *file);

/*  NO MORE THEN 1e5 !!!!!  caveat: stack overflow  */

#define BUNCH_SZ (int) 10

#define BUNCH_NUM 10

#define EPOCHS_NUM 10

void timing_search (BNodePtr test_root, char test_keys[BUNCH_SZ*EPOCHS_NUM][1], int bunch_num);
void timing_insertion (BNodePtr *test_root, char test_keys[BUNCH_SZ*EPOCHS_NUM][1], int bunch_num);
void timing_deletion (BNodePtr *test_root, char test_keys[BUNCH_SZ*EPOCHS_NUM][1], int bunch_num);
void timing_traversal (BNodePtr test_root, char test_keys[BUNCH_SZ*EPOCHS_NUM][1], int bunch_num);

const char *msgs [] = {"\n0 - quit", "1 - insert", "2 - delete by key+version",
                       "3 - find by key", "4 - find max", "5 - traverse", 
                       "6 - import", "7 - print", "8 - timing tree"};
const size_t msgc = sizeof msgs / sizeof msgs[0];

int (*fptr[]) (BNodePtr*, FILE*)  = {NULL, insert_tree, delete_tree, 
                                      find_tree, findmax_tree, traverse_tree,
                                      import_tree, print_tree, timing_tree};

int main () {

    FILE *file = stdin;
    BNodePtr root = NULL;
    int fn_num = 0;
    printf ("B23 tree UI.\n");

    while ((fn_num = option_choice (msgs, msgc, file))) {
        if (fptr[fn_num](&root, file) == eof)
            break;
    }

    fclose (file);
    free_bt (&root);
    puts ("quit");
    return 0;
}

int insert_tree (BNodePtr *root, FILE *file) {
    
    char key [51]= "", val [51] = "";
    int status = -1;
    const char *s = "Enter key of item to insert (50 symbols max): \n";

    printf ("%s", s);
    
    status = fscanf (file, "%50s", key);
    if (status == ERREOF) 
        return 0;

    s = "Enter value of item to insert (50 symbols max): \n";

    printf ("%s", s);

    status = fscanf (file, "%50s", val);
    if (status == ERREOF) 
        return 0;     

    switch (insert_bt (root, key, val)) {
        case ERRSUC:
            printf ("\nItem inserted successfully.\n");
            break;
        case ERRDUP:
            printf ("Duplicate key/value.\n");
            break;
        default:
            printf (RED("Unknown error."));
    }

    return 1; 
};

int delete_tree (BNodePtr *root, FILE *file) {

    char key [51]= "", val [51] = "";
    size_t ver = 0;
    int status = -1;
    const char *s = "Enter key of item to delete (50 symbols max): \n";

    printf ("%s", s);
     
    status = fscanf (file, "%50s", key);
    if (status == ERREOF) 
        return 0;

    /*s = "Enter version of item to delete: \n";

    printf ("%s", s);
    
    status = get_int_file (file, &ver, INT_MAX, 0);
    if (status == ERREOF) 
        return 0;  */

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

    return 1; 
};

int find_tree (BNodePtr *root, FILE *file) {
    BNodePtr node = NULL;
    size_t pos = 0;

    char key [51]= "", val [51] = "";
    int status = -1;
    const char *s = "Enter key of item to find: \n";

    printf ("%s", s);
    
    status = scanf ("%50s", key);
    if (status == ERREOF) 
        return 0;

    node = find_bt (*root, key, &pos);

    if (node) 
        colored_print_bt (*root, key);
    else 
        printf ("No such key\n");
    
    return 1;
};

int findmax_tree (BNodePtr *root, FILE *file) {
    BNodePtr node = find_max (*root);

    if (node) {
        size_t pos = node->csize - 1;
        printf (" %s (%lu) \n", node->info[pos]->key, node->info[pos]->csize);
    }
    else
        puts ("Void tree.");

    return 1;
};

int traverse_tree (BNodePtr *root, FILE *file) {
/*    size_t key = 0;
    int status = -1;
    const char *s = "Enter key to traverse from or \"a\" to traverse all tree: \n";

    printf ("%s", s);
    char ch = 0;
    scanf ("%c", &ch);

    if (ch != 'a') {
        ungetc (ch, stdin);
        status = get_int_file (file, &key, INT_MAX, 0);
        if (status == ERREOF) 
            return 0;
    } else {
        key = NO_KEY;
    }

    printf ("\nTraverse:\n");
    traverse_bst (*root, key);
    printf ("\n");*/
    return 1;
}

int import_tree (BNodePtr *root, FILE *file) {
    FILE *import = user_file ();

    if (!import) {
        puts ("quit");
        return 0;
    }

    while (insert_tree (root, import) != 0);

    fclose (import);

    return 1;
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

    /*int status = -1;
    const char *s = "\nUnwrap some branch? (y/n): \n";
    while ((status = user_choice (s)) == ERRSUC) 
        find_tree (root, file);

    if (status == ERREOF)
        return 0;
    else */
        return 1;
};


int timing_tree (BNodePtr *root, FILE *file)
{
    BNodePtr test_root = NULL;
    
    char test_keys[BUNCH_SZ*EPOCHS_NUM][1] = {};
    
    srand(time(NULL));
    
    for (int epochs_num = 0; epochs_num < EPOCHS_NUM; epochs_num++) {
        for (int bunch_num = 1; bunch_num <= BUNCH_NUM; bunch_num++) {

            int num_of_keys = BUNCH_SZ*bunch_num;

            /*  Fill test array.  */
            for (int i = 0; i < BUNCH_SZ; ++i) {
                int rand_int = rand();
                test_keys[i][0] = (char) rand_int;
            }

            /*  Fill test root  */
            for (int i = 0; i < num_of_keys; ) {
                int rand_int = rand();
                char rand_key [1] = {(char) rand_int};
                /*  ensure that the actual number of keys is incremented  */
                if (insert_bt (&test_root, rand_key, rand_key) == ERRSUC)
                    ++i;
            }

            timing_search (test_root, test_keys, bunch_num);
            //timing_insertion (&test_root, test_keys, bunch_num);
            //timing_deletion (&test_root, test_keys, bunch_num);
            //timing_traversal (test_root, test_keys, bunch_num); 

            free_bt (&test_root);   
        }
    }

    exit (0);
    return 1;
}
void timing_search (BNodePtr test_root, char test_keys[BUNCH_SZ*EPOCHS_NUM][1], int bunch_num) {
    clock_t first = 0, last = 0;
    size_t pos = 0;

    first = clock();
    for (int i = 0; i < BUNCH_SZ; ++i) 
        find_bt (test_root, test_keys[i], &pos);

    last = clock();

    printf("%d nodes_num %d time %ld\n", bunch_num, bunch_num*BUNCH_SZ, last - first);
}

void timing_insertion (BNodePtr *test_root, char test_keys[BUNCH_SZ*EPOCHS_NUM][1], int bunch_num) {
    clock_t first = 0, last = 0;
    BNodePtr buf = NULL;

    first = clock();
    for (int i = 0; i < BUNCH_SZ; ++i) 
        insert_bt (test_root, test_keys[i], 0);

    last = clock();

    printf("%d nodes_num %d time %ld\n", bunch_num, bunch_num*BUNCH_SZ, last - first);
}

void timing_deletion (BNodePtr *test_root, char test_keys[BUNCH_SZ*EPOCHS_NUM][1], int bunch_num) {
    clock_t first = 0, last = 0;
    BNodePtr buf = NULL;

    first = clock();
    for (int i = 0; i < BUNCH_SZ; ++i) 
        delete_bt (test_root, test_keys[i], 0);

    last = clock();

    printf("%d nodes_num %d time %ld\n", bunch_num, bunch_num*BUNCH_SZ, last - first);
}

/*void timing_traversal (BNodePtr test_root, char **test_keys, int bunch_num) {
    clock_t first = 0, last = 0;
    BNodePtr buf = NULL;

    first = clock();
    for (int i = 0; i < BUNCH_SZ; ++i) 
        traverse_bt (test_root, test_keys[i]);

    last = clock();

    printf("%d nodes_num %d time %ld\n", bunch_num, bunch_num*BUNCH_SZ, last - first);
}*/

