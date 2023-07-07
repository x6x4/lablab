#include "lib_bintree.h"
#include "generic.h"
#include <stdlib.h>

BstNodePtr init_node (size_t key, size_t val) {
    BstNodePtr new_node = calloc (1, sizeof *new_node);
    new_node->info = new_info (key, val);
    new_node->left = NULL;
    new_node->right = NULL;
    new_node->height = 0;

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
    
    if (find_by_key (&(new_node->par), *rooot, key) == ERRSUC) {
        size_t old_val = new_node->par->info->val;
        new_node->par->info->val = val;
        free (new_node->info);
        free (new_node);
        return old_val;
    }
    
    if (!(new_node->par)) {
        *rooot = new_node;
        new_node->prev = NULL;
        return ERRSUC;
    }

    if (new_node->par->info->key < key) {
        BstNodePtr next = next_node (*rooot, new_node->par);
        if (next)
            next->prev = new_node;
        new_node->par->right = new_node;
        new_node->prev = new_node->par;
    } else {
        new_node->par->left = new_node;
        new_node->prev = new_node->par->prev;
        new_node->par->prev = new_node;
    }
    
    return ERRSUC;
}

int delete_bst (BstNodePtr *root, size_t key) {
    BstNodePtr node = NULL, victim = NULL, next_to_victim = NULL,
    next_to_node = NULL, victim_chd = NULL, victim_par = NULL;
    int has_node_empty_child = 0;

    /*  Find node with given key.  */

    if (find_by_key (&node, *root, key) == ERRWRG) 
        return ERRWRG;

    next_to_node = find_min (node->right);
    has_node_empty_child = !(node->left) || !(node->right);

    victim = has_node_empty_child ? node : next_to_node;

    victim_chd = victim->left ? victim->left : victim->right;
    victim_par = victim->par;    

    if (!victim_par) 
        /*  Set new root  */
        *root = victim_chd;
    
    /*  Replace victim with its child.  */
    else if (victim == victim_par->left) 
        victim_par->left = victim_chd;
    else 
        victim_par->right = victim_chd;
    
    if (victim_chd)
        victim_chd->par = victim_par;
    
    if (victim != node) {
        /*  Replace node fields with victim fields  */
        node->info->key = victim->info->key;
        node->info->val = victim->info->val;
    }

    next_to_victim = next_node (*root, victim);
    if (next_to_victim)
        next_to_victim->prev = victim->prev;

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
            printf ("\n%*s└── ", 4*(height+1) - 4, ""); 
        return;
    }

    if (rooot->height != height) {
        printf ("\n");
        height = rooot->height;
    }

    if (height)
        printf ("%*s└── ", 4*height - 4, ""); 
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

    BstNodePtr buf = find_max (root);

    if (key == NO_KEY) {
        while (buf) {
            //printf ("(%lu, %lu) ", buf->info->key, buf->info->val);
            buf = buf->prev;    
        }
    } else {
        while (buf && buf->info->key > key) {
            //printf ("(%lu, %lu) ", buf->info->key, buf->info->val);
            buf = buf->prev;
        }
    }
}

BstNodePtr next_node (BstNodePtr root, BstNodePtr node) {
    BstNodePtr buf = root;
    if (!buf) 
        return NULL;

    buf = find_max (root);

    while (buf && (buf->prev != node))
        buf = buf->prev;

    return buf;
}

int find_by_key (BstNodePtr *result, BstNodePtr rooot, size_t key) {

    BstNodePtr par = rooot;
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

