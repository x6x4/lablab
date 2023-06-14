#include "src/lib_bintree.h"
#include "../../new_input/generic.h"
#include <stddef.h>
#include <stdio.h>
#include <limits.h>
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

const char *msgs [] = {"\n0 - quit", "1 - insert", "2 - delete by key",
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
    free_tree (root);
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

    InfoPtr info = new_info (key, val);

    insert_bt (root, *root, info);

    printf ("Item inserted successfully.\n");

    return 1; 
};

int delete_tree (BNodePtr *root, FILE *file) {

    char key [51]= "", val [51] = "";
    int status = -1;
    const char *s = "Enter key of item to delete (50 symbols max): \n";

    printf ("%s", s);
    
    status = fscanf (file, "%50s", key);
    if (status == ERREOF) 
        return 0;

    switch (delete_bt (root, key)) {
        case ERRSUC:
            printf ("\nItem deleted successfully.\n");
            break;
        case ERRWRG:
            printf ("No such key.\n");
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
        printf ("(%s, %s)\n", node->info[pos]->key, node->info[pos]->val);
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
    print_bt (*root);
    printf ("\n");
    return 1;
};

/*  NO MORE THEN 1e5 !!!!!  SO caught  */
#define KEYS_TO_FIND_NUM (int) 1e5
/*  no more then 1e3. all slows down */
#define KEYS_IN_BST (int) 1e3

int timing_tree (BNodePtr *root, FILE *file)
{/*
    BNodePtr temp_root = NULL;
    BNodePtr key_root = NULL;
    int init_epochs = 20, epochs = init_epochs, keys_to_find[KEYS_TO_FIND_NUM], 
           rand_key, keys_in_bst = KEYS_IN_BST, i, m;
    clock_t first, last;
    srand(time(NULL));

    while (epochs-- > 0){
        for (i = 0; i < KEYS_TO_FIND_NUM; ++i)
            keys_to_find[i] = (rand()%1000) * (rand()%1000);
        for (i = 0; i < keys_in_bst; ){
            rand_key = (rand()%1000) * (rand()%1000);
            if (insert_bt (&temp_root, rand_key, 0) == ERRSUC)
                ++i;
        }

        m = 0;
        first = clock();
        for (i = 0; i < KEYS_TO_FIND_NUM; ++i) {
            if (find_by_key (&key_root, temp_root, keys_to_find[i]) == ERRSUC)
                ++m;
        }
        last = clock();

        printf("%d items was found\n", m);
        printf("test #%d, number of nodes = %d, time = %d\n", init_epochs - epochs, 
        (init_epochs - epochs)*keys_in_bst, last - first);
        
    }
    free_bst (temp_root);*/
    return 1;
}