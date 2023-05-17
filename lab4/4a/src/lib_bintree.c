#include "lib_bintree.h"
#include "../../../../lablab/new_input/generic.h"
#include <stdlib.h>

int insert_bintree (BinNodePtr *rooot, size_t key, size_t val) {
    BinNodePtr new_node = calloc (1, sizeof *new_node);
    
    new_node->key = key;
    new_node->val = val;
    new_node->left = NULL;
    new_node->right = NULL;
    
    if (find_by_key (&(new_node->par), *rooot, key) == ERRWRG) {
        new_node->par->val = val;
        return ERRWRG;
    }
    
    if (!(new_node->par)) {
        *rooot = new_node;
        new_node->prev = NULL;
        new_node->height = 0;
        return ERRSUC;
    }

    new_node->prev = (new_node->par->key < key) ? new_node->par : new_node->par->prev;
    if (new_node->par->key < key) {
        new_node->par->right = new_node;
        new_node->prev = new_node->par;
    } else {
        new_node->par->left = new_node;
        new_node->prev = new_node->par->prev;
        new_node->par->prev = new_node;
    }
    new_node->height = new_node->par->height + 1;
    
    return ERRSUC;
}

int delete_bintree (BinNodePtr *rooot, size_t key) {
    BinNodePtr node, buf = *rooot;

    if (find_by_key (&node, *rooot, key) != ERRWRG) 
        return ERRWRG;

    while (buf->right && (buf->right->key < key)) {
        buf = buf->right;
    }

    while (buf != NULL)
    {
        if (buf->prev == node)
            break;

        buf = buf->prev;
    }

    buf->prev = node->prev;

    if ( (!(node->left)) && (!(node->right)) )  {

        if (is_left (node)) {
            node->par->left = NULL;
        } else {
            node->par->right = NULL;
        }

        free (node);
        return ERRSUC;
    }

    if (is_left (node)) 
        node->par->left = node->prev;
    else 
        node->par->right = node->prev;

    if (is_left (node->prev)) 
        node->prev->par->left = NULL;
    else 
        node->prev->par->right = NULL;

    free (node);

    return ERRSUC;
    
}

int is_left (BinNodePtr node) {
    if (node->par->key < node->key) {
        return 0;
        
    } else {
        return 1;
    }
}

void print_bintree (BinNodePtr rooot, size_t height) {

    static size_t two_void_leaves = 0;

    if (!rooot) {
        if (!two_void_leaves)
            printf ("\n%*s└── ", 4*(height+1) - 4, ""); 
        return;
    }

    if (rooot->height != height) {
        printf ("\n");
        height = rooot->height;
    }

    if (height)
        printf ("%*s└── ", 4*height - 4, ""); 
    printf ("%d", rooot->key);

    if ( (!(rooot->left)) && (!(rooot->right)) ) 
        two_void_leaves = 1;
    else 
        two_void_leaves = 0;

    print_bintree (rooot->left, height);

    if ( (!(rooot->left)) && (!(rooot->right)) ) 
        two_void_leaves = 1;
    else 
        two_void_leaves = 0;

    print_bintree (rooot->right, height);
}

void traverse_bintree (BinNodePtr node, size_t key) {

    BinNodePtr buf = node;

    while (buf->right && (buf->right->key < key)) {
        buf = buf->right;
    }

    if (buf == node) 
        return;

    while (buf) {
        printf ("%d ", buf->key);
        buf = buf->prev;
    }
}

int find_by_key (BinNodePtr *result, BinNodePtr rooot, size_t key) {

    BinNodePtr par = rooot;
    BinNodePtr buf = par;

	while (par){
		if (par->key == key) {
            *result = par;
            return ERRWRG;
        }
		
        buf = par;
		par = (par->key < key) ? par->right : par->left;
	}

    *result = buf;
	
    return ERRSUC;
}

void free_bintree (BinNodePtr root) {
    if (root) {
        free_bintree (root->left);
        free_bintree (root->right);
        free (root);
    }
    root = NULL;
}