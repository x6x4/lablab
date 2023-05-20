#include "lib_bintree.h"
#include "../../../new_input/generic.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

enum {
    ERRFULL = 5,
    ERRFREE, 
    ERRNEW
};

int search_bt (BNodePtr rooot, char *key, size_t *pos, BNodePtr *node) {
    /*  start from root  */
    BNodePtr buf = rooot, par = buf;
    int i = 0;
    size_t not_full = 0;
    
    while (buf) {
        par = buf;
        /*  check all keys in vector  */
        for (i = 0; i < KEYS_NUM; ++i) {

            /*  void place in the end of node. 
                if it's here, key obviously need this place  */
            if (buf->info[i] == NULL) {
                *pos = i;
                *node = buf;
                return ERRFREE;
            }

            /*  if key matches  */
            if (strcmp (buf->info[i]->key, key) == 0) {
                /*  nothing interesting, 
                    just return found node and key position  */
                *pos = i;
                *node = buf;
                return ERRSUC;
            } 
            
            /*  if i-th key is greater, go to i-th child  */
            else if (strcmp (key, buf->info[i]->key) < 0) {

                for (size_t i = 0; i < CHILD_NUM; i++) {
                    if (!(buf->info[KEYS_NUM - 1]))
                        not_full ++;
                }

                buf = buf->child[i];
                /*  If there is threat of */
                if (buf == NULL && (!not_full)) {
                    *node = par;
                    *pos = i;
                    return ERRNEW;
                }
                break;
            }
            /*  if key is greater than rightmost node, 
                go to rightmost child  */ 
            else if (i == KEYS_NUM - 1) {
                buf = buf->child[KEYS_NUM];
                if (buf == NULL) {
                    *node = par;
                    *pos = KEYS_NUM;
                    return ERRNEW;
                }
            }
        }
    }

    /*  no free space, just suggestion for insertion  */
    *node = par;
    /*  suggested number in child node  */
    if (i == KEYS_NUM)
        i--;
    *pos = i;

    return ERRFULL;
}

int insert_bt (BNodePtr *root, char *key, char *val) {

    size_t pos = 0; BNodePtr node = NULL;

    Info info = calloc (1, sizeof *info);  
    info->key = strdup (key);
    info->val = strdup (val);

    while (node != *root) {
        
        switch (cond_insert (root, info->key, info->val, &pos, &node)) {
            case ERRSUC:
                return ERRSUC;

            case ERRFULL:
                break;
        }

        //  keep new full node info
        info->key = strdup (node->info[pos]->key);
        info->val = strdup (node->info[pos]->val);
        //  update full node
        node->info[pos] = new_info (key, val);
    }

    *root = init_node (info->key, info->val);
    (*root)->child[0] = node;

    free (info);
    
    return ERRSUC;
}

int cond_insert (BNodePtr *root, char *key, char *val, size_t *pos, BNodePtr *node) {

    /*  Get place for insertion or handle duplication.  */

    switch (search_bt (*root, key, pos, node)) {
        case ERRSUC:
            printf ("Duplicate key\n");
            return ERRWRG;
        
        case ERRFREE:
            (*node)->info[*pos] = new_info (key, val);
            return ERRSUC;

        case ERRFULL: 
            if ((*node) == NULL) {
                *root = init_node (key, val);
                return ERRSUC;
            }
            else 
                return ERRFULL;

        case ERRNEW:
            (*node)->child[*pos] = init_node (key, val);
            return ERRSUC;

        default:
            printf ("Something wrong, try again\n");
            return ERRWRG;
    }
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

Info new_info (char *key, char *val) {
    Info info = calloc (1, sizeof *info);
    info->key = strdup (key);
    info->val = strdup (val);

    return info;
}
