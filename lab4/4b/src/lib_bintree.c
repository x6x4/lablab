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

int insert_bt (BNodePtr *root, char *key, char *val) {

    size_t pos = 0;

    /*  if root is void  */
    if ( !root ) {
        *root = init_node (key, val);
        (*root)->par = NULL;
        return ERRSUC;
    }

    BNodePtr cnode = *root, par = (*root)->par;

    /*  Keys.  */

    while (1) {

        for (int i = 0; i < KEYS_NUM; i++) {

            /*  If desired keyplace is free (all prev nodes full).  */
            if ( cnode->info[i] == NULL ) {
                cnode->info[i] = new_info (key, val);
                cnode->par = par;
                return ERRSUC;
            }

            /*  Move down or update.  */
            if ( LTE (key, cnode->info[i]->key) || (i == KEYS_NUM - 1)) {
                par = cnode;
                cnode = cnode->child[i];
                /*  If desired node exists.  */
                if (cnode)
                    break;
                /*  We reached leaf and it is full.  */
                else {
                    if (split_leaf(root, cnode, key, val) == ERRSUC)
                        return ERRSUC;
                }

            }

        }

    }

}



int split_leaf (BNodePtr *root, BNodePtr node, char *key, char *val) {

    BNodePtr right_node = calloc (1, sizeof *right_node);
    InfoPtr mid_info = NULL;
    BNodePtr mid_node = calloc (1, sizeof *mid_node);
    BNodePtr par = node->par;

    /*  key lt 1st key  */
    if (LTE(key, node->info[0]->key)) {
        /*  Standalone node.  */
        mid_info = new_info (node->info[0]->key, node->info[0]->val);

        right_node->info[0] = new_info (node->info[1]->key, node->info[1]->val);
        right_node->child[0] = node->child[1];
        right_node->child[1] = node->child[2];
        right_node->child[2] = NULL;

        free_info (node->info[0]);
        node->info[0] = new_info (key, val);
    }
    
    /*  key lt 2nd key  */
    else if (LTE(key, node->info[1]->key)) {
        mid_info = new_info (key, val);

        right_node->info[0] = new_info (node->info[1]->key, node->info[1]->val);
        right_node->child[0] = node->child[1];
        right_node->child[1] = node->child[2];
        right_node->child[2] = NULL;
       
        free_info (node->info[1]);
    }

    /*  key gt 2nd key  */
    else if (LTE(node->info[1]->key, key)) {

        mid_node->info[0] = new_info (node->info[1]->key, node->info[1]->val);
        mid_node->child[0] = node->child[1];
        mid_node->child[1] = node->child[2];
        mid_node->child[2] = NULL;

        right_node = init_node (key, val);
        free_info (node->info[1]);
    }

    /*  par node is empty*/
    if (node == *root) {
        *root = mid_node;
        mid_node->child[0] = node;
        node->par = mid_node;
        mid_node->child[1] = right_node;
        right_node->par = mid_node;
        return ERRSUC;
    }

    /*  par node isn't full, 1 key */
    if (node->par->info[1] == NULL) {
        if (LTE(node->par->info[0]->key, mid_info->key)) {
            node->par->info[1] = mid_info;
            node->par->child[2] = right_node;
        }
        else {
            node->par->info[1] = node->par->info[0];
            node->par->info[0] = mid_info;
            node->par->child[2] = node->par->child[1];
            node->par->child[1] = right_node;
        }

        return ERRSUC;
    }
    /*  par node is full, 2 keys  */
    else {
        /*  Fill mid_node.  */

        mid_node = init_node(mid_info->key, mid_info->val);
        free_info (mid_info);
        mid_node->child[0] = node;
        node->par = mid_node;
        mid_node->child[1] = right_node;
        right_node->par = mid_node;
    }
}

int split_node () {

}

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
