#include "src/lib_bintree.h"
#include "src/generic.h"
#include <stdio.h>
#include <limits.h>
#include <time.h>

int insert_tree (BstNodePtr *root, FILE *file);
int delete_tree (BstNodePtr *root, FILE *file);
int find_tree (BstNodePtr *root, FILE *file);
int findmin_tree (BstNodePtr *root, FILE *file);
int traverse_tree (BstNodePtr *root, FILE *file);
int import_tree (BstNodePtr *root, FILE *file);
int print_tree (BstNodePtr *root, FILE *file);
int timing_tree (BstNodePtr *root, FILE *file);

const char *msgs [] = {"\n0 - quit", "1 - insert", "2 - delete by key",
                       "3 - find by key", "4 - find min", "5 - traverse", 
                       "6 - import", "7 - print", "8 - timing tree"};
const size_t msgc = sizeof msgs / sizeof msgs[0];

int (*fptr[]) (BstNodePtr*, FILE*)  = {NULL, insert_tree, delete_tree, 
                                      find_tree, findmin_tree, traverse_tree,
                                      import_tree, print_tree, timing_tree};

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
    free_bst (root);
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

    if (find_by_key (&node, *root, key) == ERRSUC) 
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
    (*root)->height = 0;
    set_height (root);
    print_bst (*root, 0);
    printf ("\n");
    return 1;
};

/*  NO MORE THEN 1e5 !!!!!  SO caught  */
#define KEYS_TO_FIND_NUM (int) 1e3
/*  no more then 1e3. all slows down */
#define KEYS_IN_BST (int) 1e2

int timing_tree (BstNodePtr *root, FILE *file)
{
    BstNodePtr test_root = NULL;
    BstNodePtr key_root = NULL;
    int init_bunch = 10, init_epoch = 10, bunch = init_bunch,
    keys_to_find[KEYS_TO_FIND_NUM], rand_key, keys_in_bst = KEYS_IN_BST, i, m;
    clock_t first, last;
    srand(time(NULL));

    while (init_epoch-- > 0) {
        
        while (bunch-- > 0){
            for (i = 0; i < KEYS_TO_FIND_NUM; ++i)
                keys_to_find[i] = (rand()%100) * (rand()%100);
            for (i = 0; i < keys_in_bst; ){
                rand_key = (rand()%100) * (rand()%100);
                if (insert_bst (&test_root, rand_key, 0) == ERRSUC)
                    ++i;
            }

            /*  SEARCH  */
            m = 0;
            first = clock();
            for (i = 0; i < KEYS_TO_FIND_NUM; ++i) {
                if (find_by_key (&key_root, test_root, keys_to_find[i]) == ERRSUC)
                    ++m;
            }
            last = clock();

            //printf("%d items was found\n", m);
            printf("%d NN %d time %ld\n", init_bunch - bunch, 
            (init_bunch - bunch)*keys_in_bst, last - first);

            /*  INSERTION  */
            m = 0;
            first = clock();
            for (i = 0; i < KEYS_TO_FIND_NUM; ++i) {
                if (insert_bst (&test_root, keys_to_find[i], 0) == ERRSUC)
                    ++m;
            }
            last = clock();

            //printf("%d items was inserted\n", m);
            printf("%d NN %d time %ld\n", init_bunch - bunch, 
            (init_bunch - bunch)*keys_in_bst, last - first);

            /*  DELETION  */
            m = 0;
            first = clock();
            for (i = 0; i < KEYS_TO_FIND_NUM; ++i) {
                if (delete_bst (&test_root, keys_to_find[i]) == ERRSUC)
                    ++m;
            }
            last = clock();

            //printf("%d items was deleted\n", m);
            printf("%d NN %d time %ld\n", init_bunch - bunch, 
            (init_bunch - bunch)*keys_in_bst, last - first);

            /*  TRAVERSAL  */
            m = 0;
            first = clock();
            for (i = 0; i < KEYS_TO_FIND_NUM; ++i) {
                traverse_bst (test_root, keys_to_find[i]);
                ++m;
            }
            last = clock();

            //printf("%d items was traversed\n", m);
            printf("%d NN %d time %ld\n", init_bunch - bunch, 
            (init_bunch - bunch)*keys_in_bst, last - first);
            
        }
        bunch = init_bunch;
        
    }
    free_bst (test_root);

    return 1;
}
