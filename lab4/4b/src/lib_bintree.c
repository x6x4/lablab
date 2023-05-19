#include "lib_bintree.h"
#include "../../../new_input/generic.h"
#include <stdlib.h>
#include <string.h>

enum {
    ERRFULL = 5,
    ERRFREE
};

int search_bt (BNodePtr rooot, char *key, size_t *pos, BNodePtr *node) {
    /*  start from root  */
    BNodePtr buf = rooot, par = buf;
    int i = 0;
    
    while (buf) {
        buf = par;
        /*  check all keys in vector*/
        for (i = 0; i < KEYS_NUM; i++) {

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
                buf = buf->child[i];
                break;
            }
            /*  if key is greater than rightmost node, 
                go to rightmost child  */ 
            else if (i == KEYS_NUM - 1) 
                buf = buf->child[KEYS_NUM];
        }
    }

    /*  no free space, just suggestion for insertion  */
    *node = par;
    /*  suggested number in child node  */
    *pos = i;

    return ERRFULL;
}

int insert_bt (BNodePtr *root, char *key, char *val) {

    size_t pos = 0; BNodePtr node = NULL;

    Info info = calloc (1, sizeof *info);    

    while (node == *root) {
        
        switch (cond_insert (root, info->key, info->val, &pos, &node)) {
            case ERRSUC:
                return ERRSUC;

            case ERRFULL:
                break;
        }

        //  keep new full node info
        memcpy(info, node->info[pos], sizeof *info);
        //  update full node
        node->info[pos] = new_info (key, val);
    }

    *root = init_node (info->key, info->val);
    (*root)->child[0] = node;
    
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
        default:
            printf ("Something wrong, try again\n");
            return ERRWRG;
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
