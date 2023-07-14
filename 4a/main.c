#include "src/lib_bintree.h"
#include "src/generic.h"
#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <time.h>

int insert_tree (BstNodePtr *root, FILE *file);
int delete_tree (BstNodePtr *root, FILE *file);
int find_tree (BstNodePtr *root, FILE *file);
int findmin_tree (BstNodePtr *root, FILE *file);
int traverse_tree (BstNodePtr *root, FILE *file);
int import_tree (BstNodePtr *root, FILE *file);
int print_tree (BstNodePtr *root, FILE *file);
int timing_tree (BstNodePtr *root, FILE *file);

void timing_search (BstNodePtr test_root, int *test_keys, int bunch_num);
void timing_insertion (BstNodePtr *test_root, int *test_keys, int bunch_num);
void timing_deletion (BstNodePtr *test_root, int *test_keys, int bunch_num);
void timing_traversal (BstNodePtr test_root, int *test_keys, int bunch_num);

/*  GLOBAL VARIABLES  */

const char *msgs [] = {"\n0 - quit", "1 - insert", "2 - delete by key",
                       "3 - find by key", "4 - find min", "5 - traverse", 
                       "6 - import", "7 - print", "8 - timing tree"};
const size_t msgc = sizeof msgs / sizeof msgs[0];

int (*fptr[]) (BstNodePtr*, FILE*)  = {NULL, insert_tree, delete_tree, 
                                      find_tree, findmin_tree, traverse_tree,
                                      import_tree, print_tree, timing_tree};

/*  MAIN  */

int main () {

    FILE *file = stdin;
    BstNodePtr root = NULL;
    size_t fn_num = 0;

    while ((fn_num = option_choice (msgs, msgc, file))) {
        //  each function returns 0 if EOF
        if (!fptr[fn_num](&root, file))
            break;
    }

    fclose (file);
    free_bst (&root);
    puts ("quit");
    return 0;
}

int insert_tree (BstNodePtr *root, FILE *file) {
    
    size_t key = 0, val = 0;
    int status = -1;
    const char *s = "Enter key of item to insert: \n";

    printf ("%s", s);
    
    status = get_int_file (file, &key, INT_MAX, 0);
    if (status == ERREOF) 
        return 0;

    s = "Enter value of item to insert: \n";

    printf ("%s", s);

    status = get_int_file (file, &val, INT_MAX, 0);
    if (status == ERREOF) 
        return 0;       

    size_t ret_val = insert_bst (root, key, val);

    if (ret_val == ERRSUC) 
        printf ("Item inserted successfully.\n");
    else 
        printf ("Duplicate key. Old val: %lu\n", ret_val);

    return 1; 
};

int delete_tree (BstNodePtr *root, FILE *file) {
    size_t key = 0;
    int status = -1;
    const char *s = "Enter key of item to delete: \n";

    printf ("%s", s);
    
    status = get_int_file (file, &key, INT_MAX, 0);
    if (status == ERREOF) 
        return 0;       

    if (delete_bst (root, key) == ERRSUC) 
        printf ("Item deleted successfully.\n");
    else 
        printf ("Key not found btw.\n");

    return 1; 
};

int find_tree (BstNodePtr *root, FILE *file) {
    BstNodePtr node = NULL;

    size_t key = 0;
    int status = -1;
    const char *s = "Enter key of item to find: \n";

    printf ("%s", s);

    status = get_int_file (file, &key, INT_MAX, 0);
    if (status == ERREOF) 
        return 0;

    if (find_by_key_or_ret_par (&node, *root, key) == ERRSUC) 
        printf ("(%lu, %lu)\n", node->info->key, node->info->val);
    else 
        printf ("No key\n");

    return 1;
};

int findmin_tree (BstNodePtr *root, FILE *file) {
    BstNodePtr node = find_min (*root);
    if (node)
        printf ("(%lu, %lu)\n", node->info->key, node->info->val);

    return 1;
};

int traverse_tree (BstNodePtr *root, FILE *file) {
    size_t key = 0;
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
    printf ("\n");
    return 1;
}

int import_tree (BstNodePtr *root, FILE *file) {
    FILE *import = user_file ();

    if (!import) {
        puts ("quit");
        return 0;
    }

    while (insert_tree (root, import) != 0);

    fclose (import);

    return 1;
}

int print_tree (BstNodePtr *root, FILE *file) {
    printf ("\nTree:\n");
    if (!(*root)) {
        printf ("(void)\n");
        return 1;
    }
    (*root)->height = 0;
    set_height (root);
    print_bst (*root, 0);
    printf ("\n");
    return 1;
};

/*  Killed on 1e4  */
#define BUNCH_SZ (int) 1e3

/*  Overall EPOCHS_NUM epochs with EPOCHS_NUM bunches in each.  */
#define EPOCHS_NUM 50


int timing_tree (BstNodePtr *root, FILE *file)
{
    BstNodePtr test_root = NULL;
    
    int test_keys[BUNCH_SZ*EPOCHS_NUM];
    
    srand(time(NULL));
    
    for (int epochs_num = 0; epochs_num < EPOCHS_NUM; epochs_num++) {
        for (int bunch_num = 1; bunch_num <= EPOCHS_NUM; bunch_num++) {

            int num_of_keys = BUNCH_SZ*bunch_num;

            /*  Fill test array.  */
            for (int i = 0; i < BUNCH_SZ; ++i)
                test_keys[i] = rand();

            /*  Fill test root  */
            for (int i = 0; i < num_of_keys; ){
                int rand_key = rand();
                /*  ensure that the actual number of keys is incremented  */
                if (insert_bst (&test_root, rand_key, 0) == ERRSUC)
                    ++i;
            }

            timing_search (test_root, test_keys, bunch_num);
            timing_insertion (&test_root, test_keys, bunch_num);
            timing_deletion (&test_root, test_keys, bunch_num);
            //timing_traversal (test_root, test_keys, bunch_num); 

            free_bst (&test_root);   
        }
    }

    exit (0);
    return 1;
}
void timing_search (BstNodePtr test_root, int *test_keys, int bunch_num) {
    clock_t first = 0, last = 0;
    BstNodePtr buf = NULL;

    first = clock();
    for (int i = 0; i < BUNCH_SZ; ++i) 
        find_by_key_or_ret_par (&buf, test_root, test_keys[i]);

    last = clock();

    printf("%d nodes_num %d time %ld\n", bunch_num, bunch_num*BUNCH_SZ, last - first);
}

void timing_insertion (BstNodePtr *test_root, int *test_keys, int bunch_num) {
    clock_t first = 0, last = 0;
    BstNodePtr buf = NULL;

    first = clock();
    for (int i = 0; i < BUNCH_SZ; ++i) 
        insert_bst (test_root, test_keys[i], 0);

    last = clock();

    printf("%d nodes_num %d time %ld\n", bunch_num, bunch_num*BUNCH_SZ, last - first);
}

void timing_deletion (BstNodePtr *test_root, int *test_keys, int bunch_num) {
    clock_t first = 0, last = 0;
    BstNodePtr buf = NULL;

    first = clock();
    for (int i = 0; i < BUNCH_SZ; ++i) 
        delete_bst (test_root, test_keys[i]);

    last = clock();

    printf("%d nodes_num %d time %ld\n", bunch_num, bunch_num*BUNCH_SZ, last - first);
}

void timing_traversal (BstNodePtr test_root, int *test_keys, int bunch_num) {
    clock_t first = 0, last = 0;
    BstNodePtr buf = NULL;

    first = clock();
    for (int i = 0; i < BUNCH_SZ; ++i) 
        traverse_bst (test_root, test_keys[i]);

    last = clock();

    printf("%d nodes_num %d time %ld\n", bunch_num, bunch_num*BUNCH_SZ, last - first);
}