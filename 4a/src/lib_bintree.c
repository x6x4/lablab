#include "lib_bintree.h"
#include "generic.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

BstNodePtr init_node (size_t key, size_t val) {
    BstNodePtr new_node = calloc (1, sizeof *new_node);
    
    new_node->info = new_info (key, val);
    new_node->left = new_node->right = NULL;
    new_node->height = 0;
    new_node->prev = new_node->next = NULL;

    return new_node;
}

Info new_info (size_t key, size_t val) {
    Info info = calloc (1, sizeof *info);
    info->key = key;
    info->val = val;

    return info;
}

size_t insert_bst (BstNodePtr *rooot, size_t key, size_t val) {

    BstNodePtr new_node = init_node (key, val);
    
    /*  Duplicate.  */
    if (find_by_key_or_ret_par (&(new_node->par), *rooot, key) == ERRSUC) {
        size_t old_val = new_node->par->info->val;
        new_node->par->info->val = val;
        free (new_node->info);
        free (new_node);
        return old_val;
    }

    BstNodePtr par = new_node->par;
    
    /*  Insert root.  */
    if (!par) {
        *rooot = new_node;
        return ERRSUC;
    }

    if (key > par->info->key) {
        /*  insert to right.  */
        par->right = new_node;

        BstNodePtr next = par->next;
        
        /*  par -> new node  */
        new_node->prev = par;
        par->next = new_node;
        /*  new node -> next  */
        if (next) next->prev = new_node;
        new_node->next = next;

    } else {
        /*  insert to left.  */
        par->left = new_node;
        
        BstNodePtr prev = par->prev;

        /*  prev -> new node  */
        if (prev) prev->next = new_node;
        new_node->prev = prev;
        /*  new node -> par  */
        new_node->next = par;
        par->prev = new_node;
    }
    
    return ERRSUC;
}

int delete_bst (BstNodePtr *root, size_t key) {
    BstNodePtr node = NULL, victim = NULL, victim_chld = NULL, victim_par = NULL;

    /*  Find node with given key.  */
    if (find_by_key_or_ret_par (&node, *root, key) == ERRWRG) 
        return ERRWRG;
    
    int has_node_empty_child = !(node->left) || !(node->right);

    /*  Set core variables.  */
    victim = has_node_empty_child ? node : find_min (node->right);
    victim_chld = victim->left ? victim->left : victim->right;
    victim_par = victim->par;    

    if (!victim_par) 
        /*  Set new root  */
        *root = victim_chld;
    
    /*  Replace victim with its child.  */
    else if (victim == victim_par->left) 
        victim_par->left = victim_chld;
    else 
        victim_par->right = victim_chld;
    
    /*  Link grandchild to granddad  */
    if (victim_chld) victim_chld->par = victim_par;
    
    if (victim != node) {
        /*  Replace node fields with victim fields  */
        node->info->key = victim->info->key;
        node->info->val = victim->info->val;
    }

    BstNodePtr victim_prev = victim->prev;
    BstNodePtr victim_next = victim->next;

    /*  Link victim_prev and victim_next  */
    if (victim_prev) victim_prev->next = victim->next;
    if (victim_next) victim_next->prev = victim->prev;

    /*  Deletion itself  */
    free_nullify (victim->info);
    free_nullify (victim);

    return ERRSUC;
}

void set_height (BstNodePtr *root) {
    if ( !(*root) )
        return;

    if ((*root)->left)
        (*root)->left->height = (*root)->height + 1;
    if ((*root)->right)
        (*root)->right->height = (*root)->height + 1;

    set_height (&((*root)->left));
    set_height (&((*root)->right));
}  

void print_bst (BstNodePtr rooot, size_t height) {

    static size_t two_void_leaves = 0;

    if (!rooot) {
        if (!two_void_leaves)
            printf ("\n%*s└── ", (int) (4*(height+1) - 4), ""); 
        return;
    }

    if (rooot->height != height) {
        printf ("\n");
        height = rooot->height;
    }

    if (height)
        printf ("%*s└── ", (int) (4*height - 4), ""); 
    printf ("(%lu, %lu)", rooot->info->key, rooot->info->val);

    if ( (!(rooot->left)) && (!(rooot->right)) ) 
        two_void_leaves = 1;
    else 
        two_void_leaves = 0;

    print_bst (rooot->left, height);

    if ( (!(rooot->left)) && (!(rooot->right)) ) 
        two_void_leaves = 1;
    else 
        two_void_leaves = 0;

    print_bst (rooot->right, height);
}

void traverse_bst (BstNodePtr root, size_t key) {

    if (!root) {
        //printf ("(void)");
        return;
    }

    BstNodePtr buf = find_max (root);

    if (key == NO_KEY) {
        while (buf) {
            printf ("%lu: %lu\n", buf->info->key, buf->info->val);
            buf = buf->prev;    
        }
    } 
    else {
        if (!(buf && buf->info->key > key)) {
            //printf ("(no greater keys)");
            return;
        }

        while (buf && buf->info->key > key) {
            //printf ("(%lu, %lu) ", buf->info->key, buf->info->val);
            buf = buf->prev;
        }
    }
}

/*  If key not found returns in par possible parent.  */
int find_by_key_or_ret_par (BstNodePtr *result, BstNodePtr root, size_t key) {

    BstNodePtr par = root;
    BstNodePtr buf = par;

	while (par){
		if (par->info->key == key) {
            *result = par;
            return ERRSUC;
        }
		
        buf = par;
		par = (par->info->key < key) ? par->right : par->left;
	}

    *result = buf;
	
    return ERRWRG;
}

BstNodePtr find_min (BstNodePtr rooot) {
    BstNodePtr buf = rooot;
    if (!buf) 
        return NULL;

    while (buf->left) 
        buf = buf->left;

    return buf;
}

BstNodePtr find_max (BstNodePtr rooot) {
    BstNodePtr buf = rooot;
    if (!buf) 
        return NULL;

    while (buf->right) 
        buf = buf->right;

    return buf;
}

void free_bst (BstNodePtr *root) {
    if (*root) {
        free_bst (&((*root)->left));
        free_bst (&((*root)->right));
        free_nullify ((*root)->info);
        free_nullify (*root);
    }
}

size_t is_leaf (BstNodePtr root) {
    return !(root->left) && !(root->right);
}

