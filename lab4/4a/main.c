#include "src/lib_bintree.h"
#include "../../new_input/generic.h"
#include <stdio.h>
#include <limits.h>

int insert_tree (BinNodePtr *root, FILE *file);
int delete_tree (BinNodePtr *root, FILE *file);
int find_tree (BinNodePtr *root, FILE *file);
int findmin_tree (BinNodePtr *root, FILE *file);
int traverse_tree (BinNodePtr *root, FILE *file);
int print_tree (BinNodePtr *root, FILE *file);

const char *msgs [] = {"\n0 - quit", "1 - insert", "2 - delete by key",
                       "3 - find by key", "4 - find min", "5 - traverse", "6 - print"};
const size_t msgc = sizeof msgs / sizeof msgs[0];

int (*fptr[]) (BinNodePtr*, FILE*)  = {NULL, insert_tree, delete_tree, find_tree, 
                                      findmin_tree, traverse_tree, print_tree};

int main () {

    FILE *file = stdin;
    //user_file ();
    /*if (!file) {
        puts ("quit");
        return 0;
    }*/

    BinNodePtr root = NULL;
    
    size_t fn_num = 0;

    while (fn_num = option_choice (msgs, msgc, file)) {
        //  each function returns 0 if EOF
        if (!fptr[fn_num](&root, file))
            break;
    }

    fclose (file);
    free_bintree (root);
    puts ("quit");
    return 0;
}

int insert_tree (BinNodePtr *root, FILE *file) {
    
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

    if (insert_bintree (root, key, val) == ERRSUC) 
        printf ("Item inserted successfully.\n");
    else 
        printf ("Duplicate key btw.\n");

    return 1; 
};

int delete_tree (BinNodePtr *root, FILE *file) {
    size_t key;
    int status = -1;
    const char *s = "Enter key of item to delete: \n";

    printf ("%s", s);
    
    status = get_int_file (file, &key, INT_MAX, 0);
    if (status == ERREOF) 
        return 0;       

    if (delete_bintree (root, key) == ERRSUC) 
        printf ("Item deleted successfully.\n");
    else 
        printf ("Key not found btw.\n");

    return 1; 
};
int find_tree (BinNodePtr *root, FILE *file) {

};
int findmin_tree (BinNodePtr *root, FILE *file) {

};

int traverse_tree (BinNodePtr *root, FILE *file) {
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
    traverse_bintree (*root, key);
    printf ("\n");
    return 1;
}

int print_tree (BinNodePtr *root, FILE *file) {
    printf ("\nTree:\n");
    print_bintree (*root, 0);
    printf ("\n");
    return 1;
};