#include "lib_bintree.h"
#include "../../../../lablab/new_input/generic.h"
#include <stdlib.h>

size_t insert_bst (BstNodePtr *rooot, size_t key, size_t val) {
    BstNodePtr new_node = calloc (1, sizeof *new_node);
    
    new_node->key = key; // mb init?
    new_node->val = val;
    new_node->left = NULL;
    new_node->right = NULL;
    new_node->height = 0;
    
    if (find_by_key (&(new_node->par), *rooot, key) == ERRSUC) {
        size_t old_val = new_node->par->val;
        new_node->par->val = val;
        free (new_node);
        return old_val;
    }
    
    if (!(new_node->par)) {
        *rooot = new_node;
        new_node->prev = NULL;
        return ERRSUC;
    }

    if (new_node->par->key < key) {
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

int delete_bst (BstNodePtr *rooot, size_t key) {
    BstNodePtr node, true_deleted = NULL, next_true_deleted = NULL,
    true_subtree = NULL, true_par = NULL;

    if (find_by_key (&node, *rooot, key) == ERRWRG) 
        return ERRWRG;

    /*  Ok, we found node with given key.  */

    if ( !(node->left) || !(node->right) ) 
        true_deleted = node;
    else {
        true_deleted = find_min (node->right);
        next_true_deleted = next_node (*rooot, true_deleted);
    }

    /*  So in fact we delete node with only 1 child.  */
    
    if (true_deleted->left)
        true_subtree = true_deleted->left;
    else
        true_subtree = true_deleted->right;

    true_par = true_deleted->par;

    /*  We set subtree and partree for our node.  */

    if (true_subtree)
        true_subtree->par = true_par;

    if (!true_par) 
        rooot = &true_subtree;
    else if (true_deleted == true_par->left) 
        true_par->left = true_subtree;
    else 
        true_par->right = true_subtree;
    
    if (true_deleted != node) {
        if (next_true_deleted)
            next_true_deleted->prev = node;
        
        node->key = true_deleted->key;
        node->val = true_deleted->val;
    }

    free (true_deleted);

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
    printf ("(%lu, %lu)", rooot->key, rooot->val);

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
            printf ("(%lu, %lu) ", buf->key, buf->val);
            buf = buf->prev;    
        }
    } else {
        while (buf->key > key) {
            printf ("(%lu, %lu) ", buf->key, buf->val);
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
		if (par->key == key) {
            *result = par;
            return ERRSUC;
        }
		
        buf = par;
		par = (par->key < key) ? par->right : par->left;
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


void free_bst (BstNodePtr root) {
    if (root) {
        free_bst (root->left);
        free_bst (root->right);
        free (root);
    }
    root = NULL;
}