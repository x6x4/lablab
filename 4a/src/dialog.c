#include "dialog.h"
#include "lib_bintree.h"
#include <stddef.h>
#include <time.h>



/*  DIALOG  */

void dialog (Dialog_info info, int (*fptr[]) (BstNodePtr*, FILE*), BstNodePtr *bin_tree, FILE* file) {

    int fn_num = 0;
    
    puts (info->greeting);

    while ((fn_num = option_choice (info->msgs, info->msgc, file))) {
        if (fptr[fn_num](bin_tree, file) == ERREOF)
            break;
    }

    puts (info->exit_msg);
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



/*||||||||||||||||||||||||| < GENERAL FUNCTIONS > |||||||||||||||||||||||||*/

static size_t dump_num = 0;
int dump_tree (BstNodePtr *root, FILE *file) {

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

void dump_node (BstNodePtr node, FILE *file) {

    if (!node) {
        fprintf (file, "NULL\n");
        return;
    }

    fprintf (file, "node_%p [label=\"", node);
    fprintf (file, "%zu", node->info->key);
    fprintf (file, "\"];\n");

    size_t child_num = 0;

    if (is_leaf (node))
        return;

    if (node->left) {
        dump_node (node->left, file);
        fprintf (file, "node_%p -- node_%p\n", node, node->left);
    }
    else {
        fprintf (file, "node_%p_null [label=\"NULL\"];\n", node);
        fprintf (file, "node_%p -- node_%p_null\n", node, node);
    }

    if (node->right) {
        dump_node (node->right, file);
        fprintf (file, "node_%p -- node_%p\n", node, node->right);
        child_num++;
    } 
    else {
        fprintf (file, "node_%p_null [label=\"NULL\"];\n", node);
        fprintf (file, "node_%p -- node_%p_null\n", node, node);
    }
}


int insert_tree (BstNodePtr *root, FILE *file) {
    
    size_t key = 0, val = 0;
    int status = -1;
    const char *s = "Enter key of item to insert: \n";

    printf ("%s", s);
    
    status = get_sizet_file (file, &key, INT_MAX, 0);
    if (status == ERREOF) 
        return 0;

    s = "Enter value of item to insert: \n";

    printf ("%s", s);

    status = get_sizet_file (file, &val, INT_MAX, 0);
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
    
    status = get_sizet_file (file, &key, INT_MAX, 0);
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

    status = get_sizet_file (file, &key, INT_MAX, 0);
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
        status = get_sizet_file (file, &key, INT_MAX, 0);
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

/*  TIMING  */

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
                if (insert_bst (&test_root, (size_t) rand_key, 0) == ERRSUC)
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
        find_by_key_or_ret_par (&buf, test_root, (size_t) test_keys[i]);

    last = clock();

    printf("%d nodes_num %d time %ld\n", bunch_num, bunch_num*BUNCH_SZ, last - first);
}

void timing_insertion (BstNodePtr *test_root, int *test_keys, int bunch_num) {
    clock_t first = 0, last = 0;

    first = clock();
    for (int i = 0; i < BUNCH_SZ; ++i) 
        insert_bst (test_root, (size_t) test_keys[i], 0);

    last = clock();

    printf("%d nodes_num %d time %ld\n", bunch_num, bunch_num*BUNCH_SZ, last - first);
}

void timing_deletion (BstNodePtr *test_root, int *test_keys, int bunch_num) {
    clock_t first = 0, last = 0;

    first = clock();
    for (int i = 0; i < BUNCH_SZ; ++i) 
        delete_bst (test_root, (size_t) test_keys[i]);

    last = clock();

    printf("%d nodes_num %d time %ld\n", bunch_num, bunch_num*BUNCH_SZ, last - first);
}

void timing_traversal (BstNodePtr test_root, int *test_keys, int bunch_num) {
    clock_t first = 0, last = 0;

    first = clock();
    for (int i = 0; i < BUNCH_SZ; ++i) 
        traverse_bst (test_root, (size_t) test_keys[i]);

    last = clock();

    printf("%d nodes_num %d time %ld\n", bunch_num, bunch_num*BUNCH_SZ, last - first);
}
