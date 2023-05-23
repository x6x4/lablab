#include "lib_bintree.h"
#include "../../../new_input/generic.h"
#include <limits.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

enum {
    ERRFULL = 5,
    ERRFREE, 
    ERRNEW
};

#define LTE(s1,s2) ((-strcmp (s1, s2)) >= 0)

void set_height (BNodePtr *root) {
    if ( !(*root) )
        return;

    for (size_t i = 0; i < CHILD_NUM; i++) {
        if ((*root)->child[i])
            (*root)->child[i]->height = (*root)->height + 1;
    }

    for (size_t i = 0; i < CHILD_NUM; i++) 
        set_height (&((*root)->child[i]));
} 

void print_bt (BNodePtr root, size_t height) {

    static size_t null_children = 0;

    if (!root) {
        if (!null_children)
            printf ("\n%*s└── ", 4*(height+1) - 4, ""); 
        return;
    }

    if (root->height != height) {
        printf ("\n");
        height = root->height;
    }

    if (height) 
        printf ("%*s└── ", 4*height - 4, ""); 

    for (size_t i = 0; i < KEYS_NUM; i++) {
        if (root->info[i])
            printf ("(%s, %s) ", root->info[i]->key , root->info[i]->val);
    }

    for (size_t i = 0; i < CHILD_NUM; i++) {

        null_children = 1;

        for (size_t j = 0; j < CHILD_NUM; j++) {
            if (root->child[j])
                null_children = 0;
        }

        print_bt (root->child[i], height);
    }
}

/*  Constructors.  */

BNodePtr init_node (char *key, char *val) {
    BNodePtr new_node = calloc (1, sizeof *new_node);
    memset (new_node, 0, sizeof *new_node);
    new_node->info[0] = new_info (key, val);

    return new_node;
}

InfoPtr new_info (char *key, char *val) {
    InfoPtr info = calloc (1, sizeof *info);
    info->key = strdup (key);
    info->val = strdup (val);

    return info;
}

void free_info (InfoPtr info) {
    free (info->key);
    free (info->val);
    free (info);
    return;
}
