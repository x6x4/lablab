#include "lib_mini_bt.h"
#include "generic.h"
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*  TREE  */

int insert_somewhere (BNodePtr *root, Key key, char *val) {
    size_t pos = 0;
    BNodePtr node = NULL;
    node = find_bt (*root, key, &pos);

    InfoPtr info = !node ? NULL : node->info[pos];

    if (change_info (&info, key, val) == ERRDUP) 
        return ERRDUP;

    info->csize++;
    
    if (node)
        return ERRSUC;

    insert_bt (root, *root, info);
    return ERRSUC;
}

/*  INSERTION  */
void insert_bt (BNodePtr *root, BNodePtr cnode, InfoPtr info) {

    if (!(*root)) {
        *root = new_vertex (info);
        return;
    }
 
    if (is_leaf (cnode)) {
        insert_to_vertex (cnode, info);
        split_up_from_node (root, cnode);
        return;
    }

    /*  Recursive descent  */
    for (size_t i = 0; i < cnode->csize; i++) {
        if ( LTE (info->key, cnode->info[i]->key) ) {
            insert_bt (root, cnode->child[i], info);
            return;
        }
    }
    /*  go to rightmost child  */
    insert_bt (root, cnode->child[cnode->csize], info);
}

void split_up_from_node (BNodePtr *root, BNodePtr node) {
    if (node->csize < 3)
        return;

    BNodePtr left = NULL, right = NULL;
    create_at_split (node, &left, &right);
    
    if (node == *root) {
        left->par = *root;
        right->par = *root;
        fix_root_after_split (*root, (*root)->info[1], left, right);
        return;
    }

    else {
        insert_to_vertex (node->par, node->info[1]);
        
        /*  set the space of split node to null  */
        for (size_t i = 0; i < CHILD_NUM - 1; i++) {
            if (node->par->child[i] == node) {
                node->par->child[i] = NULL;
                break;
            }
        }
        
        for (size_t i = 0; i < 3; i++) {
            if (!(node->par->child[i])) {
                /*  move to make room for new children  */
                node->par->child[3] = node->par->child[2];
                node->par->child[2] = node->par->child[1];

                /*  fill released space with left and right  */
                node->par->child[i] = left;
                node->par->child[i+1] = right;    
                break;
            }
        }

        BNodePtr par = node->par;
        free_nullify (node);
        /*  move up the tree  */
        split_up_from_node (root, par);
    }

}

void create_at_split (BNodePtr node, BNodePtr *left, BNodePtr *right) {
    BNodePtr left_children [4] = {node->child[0], node->child[1], NULL, NULL};
    *left = new_bt_node (node->info[0], left_children, node->par);
    
    BNodePtr right_children [4] = {node->child[2], node->child[3], NULL, NULL};
    *right = new_bt_node (node->info[2], right_children, node->par);

    /*  set left and right as parents  */
    for (size_t i = 0; i < 2; i++) {
        if ((*left)->child[i])
            (*left)->child[i]->par = *left;
        if ((*right)->child[i])
            (*right)->child[i]->par = *right;
    }
}


/*  PRINT  */

void colored_print_bt (BNodePtr root, char *key) {
    set_height (root);
    colored_print_bt_lvl (root, 0, key);
}

void set_height (BNodePtr root) {
    if (!root)
        return;
    for (size_t i = 0; i < CHILD_NUM - 1; i++) {
        if (root->child[i])
            root->child[i]->height = root->height + 1;
    }
    for (size_t i = 0; i < CHILD_NUM - 1; i++) 
        set_height (root->child[i]);
} 

void colored_print_bt_lvl (BNodePtr root, size_t height, char *key) {
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
    for (size_t i = 0; i < root->csize; i++) {
        if (key && EQ(root->info[i]->key, key)) {
            printf (RED("%s(%lu): "), root->info[i]->key, root->info[i]->csize);
            print_ll (root->info[i]->head);
        }
        else 
            printf ("%s(%lu) ", root->info[i]->key, root->info[i]->csize);
    }
    for (size_t i = 0; i < CHILD_NUM - 1; i++) {
        null_children = 1;
        for (size_t j = 0; j < CHILD_NUM - 1; j++) {
            if (root->child[j])
                null_children = 0;
        }
        colored_print_bt_lvl (root->child[i], height, key);
    }
}


/*  SEARCH  */
BNodePtr find_bt (BNodePtr root, Key key, size_t *pos) {
    
    if (!root) 
        return NULL;

    if (find_in_vertex (root, key, pos) == ERRSUC) 
        return root;

    if (LTE(key, root->info[0]->key))
        return find_bt (root->child[0], key, pos);
    if (root->csize == 2 && LTE(key, root->info[1]->key) 
        || root->csize == 1)
        return find_bt (root->child[1], key, pos);
    if (root->csize == 2)
        return find_bt (root->child[2], key, pos);

    return NULL;
}

BNodePtr find_max (BNodePtr root) {
    if (root == NULL || root->child[0] == NULL /*leaf*/ )
        return root;
    for (size_t i = 0; i < CHILD_NUM - 1; i++) {
        if (root->child[i+1] == NULL) 
            return find_max (root->child[i]);
    }
    return find_max (root->child[CHILD_NUM-1]);
}

/*  DELETION  */
int delete_bt (BNodePtr *root, Key key, size_t ver) {
    size_t pos_node = 0, pos_max = 0;
    BNodePtr node_for_deletion = NULL;
    BNodePtr max_in_lchild = NULL;
    InfoPtr *max_key_in_lchild = NULL;
    InfoPtr *key_to_delete = NULL;

    /*  deletion from list or from tree  */
    node_for_deletion = find_bt (*root, key, &pos_node);
    if (!node_for_deletion)
        return ERRWRG;

    INode *head = &(node_for_deletion->info[pos_node]->head);

    return delete_from_ll (head, ver);

    /*  move key to leaf  */
    max_in_lchild = find_max (node_for_deletion->child[pos_node]);
    key_to_delete = &(node_for_deletion->info[pos_node]);
    
    if (max_in_lchild) {
        max_key_in_lchild = &(max_in_lchild->info[max_in_lchild->csize - 1]);
        swap (max_key_in_lchild, key_to_delete);
        node_for_deletion = max_in_lchild;
    }

    /*  delete key in leaf  */
    if (max_key_in_lchild)
        free_info (max_key_in_lchild);
    else 
        free_info (key_to_delete);

    delete_from_vertex (node_for_deletion, key);

    /*  fix bt  */
    fix_after_del (root, node_for_deletion);
    return ERRSUC;
}

void fix_after_del (BNodePtr *root, BNodePtr leaf) {

    size_t vertex_3_chld = CHILD_NUM - 1;

    if (!leaf)
        return;

    /*  the only key in the tree  */
    if (leaf->csize == 0 && leaf->par == NULL) {
        free_nullify(*root);
        return;
    }

    /*  leaf with 2 keys  */
    if (leaf->csize != 0) {
        if (leaf->par)
            fix_after_del (root, leaf->par);
        return;
    }

    /*  emptied child  */
    BNodePtr par = leaf->par;

    for (size_t i = 0; i < par->csize + 1; i++) {
        if (par->child[i]->csize == 2) {
            leaf = redistribute (leaf);
            vertex_3_chld = i;
            break;
        }
    } 

    if (vertex_3_chld == CHILD_NUM - 1 && par->csize == 2)
        leaf = redistribute (leaf);
    
    else if (vertex_3_chld == CHILD_NUM - 1)
        leaf = merge (root, leaf);

    fix_after_del (root, leaf);
}

void insert_parent_key (size_t num, BNodePtr par) {
    switch (num) {
        case 0:
        case 1:
            insert_to_vertex (par->child[0], par->info[0]);
            break;
        case 2:
            insert_to_vertex (par->child[1], par->info[1]);
    }
}

void lshift_fill (size_t num, BNodePtr par) {
    switch (num) {
        case 0:
            par->child[0] = par->child[1];
        case 1:
            par->child[1] = par->child[2];
        case 2:
            par->child[2] = NULL;
    }
}

void clear (size_t num, BNodePtr par, BNodePtr leaf) {
    switch (num) {
        case 0:
        case 1:
            delete_from_vertex (par, par->info[0]->key);
            break;
        case 2:
            delete_from_vertex (par, par->info[1]->key);
    }
    free_nullify (leaf);
}

void insert_third_child (size_t num, BNodePtr par, BNodePtr leaf) {
    BNodePtr node_to_insert = NULL;
    BNodePtr possible_par = NULL;

    for (size_t i = 0; i < CHILD_NUM - 2; i++) {
        if (leaf->child[i]) {
            node_to_insert = leaf->child[i];
            break;
        }
    }
    
    BNodePtr *place_to_insert = NULL;

    switch (num) {
        case 0:
            place_to_insert = &(par->child[0]->child[0]);
            break;
        case 1:
            place_to_insert = &(par->child[0]->child[2]);
            break;
        case 2:
            place_to_insert = &(par->child[1]->child[2]);
    }

    if (*place_to_insert)
        possible_par = (*place_to_insert)->par;

    *place_to_insert = node_to_insert;
    if (node_to_insert) 
        (*place_to_insert)->par = possible_par;
}

void rotate_right (size_t src, size_t dest, BNodePtr par) {
    /*  leave left child empty  */
    if (par->child[dest]->child[1] == NULL) {
        par->child[dest]->child[1] = par->child[dest]->child[0];
        par->child[dest]->child[0] = NULL;
    }

    size_t src_sz = par->child[src]->csize;
    /*  change infos  */
    par->child[dest]->info[0] = par->info[src];
    par->child[dest]->csize = 1;

    par->info[src] = par->child[src]->info[src_sz - 1];
    delete_from_vertex (par->child[src], par->child[src]->info[src_sz - 1]->key);

    /*  change children  */
    par->child[dest]->child[0] = par->child[src]->child[src_sz];

    par->child[src]->child[src_sz] = NULL;
    if (par->child[dest]->child[0]) 
        par->child[dest]->child[0]->par = par->child[dest];
}

void rotate_left (size_t src, size_t dest, BNodePtr par) {
    /*  leave right child empty  */
    if (par->child[dest]->child[0] == NULL) {
        par->child[dest]->child[0] = par->child[dest]->child[1];
        par->child[dest]->child[1] = NULL;
    }

    size_t src_sz = par->child[src]->csize;
    /*  change infos  */
    par->child[dest]->info[0] = par->info[src - 1];
    par->child[dest]->csize = 1;

    par->info[src - 1] = par->child[src]->info[0];
    delete_from_vertex (par->child[src], par->child[src]->info[0]->key);

    /*  change children  */
    par->child[dest]->child[1] = par->child[src]->child[0];
    for (size_t i = 0; i < src_sz; i++)
        par->child[src]->child[i] = par->child[src]->child[i+1];

    par->child[src]->child[src_sz] = NULL;
    if (par->child[dest]->child[1]) 
        par->child[dest]->child[1]->par = par->child[dest];
}

size_t set_num (BNodePtr par, BNodePtr leaf) {

    for (size_t i = 0; i < CHILD_NUM - 1; i++) {
        if (leaf == par->child[i]) 
            return i;
    }
    return CHILD_NUM - 1;
}

size_t set_v3_num (BNodePtr par, BNodePtr leaf) {

    for (size_t i = 0; i < CHILD_NUM - 1; i++) {
        if (par->child[i]->csize == 2)
            return i;
    }
    return CHILD_NUM - 1;
}

BNodePtr redistribute (BNodePtr leaf) {

    BNodePtr par = leaf->par;
    if (!par)
        return leaf;

    size_t leaf_num = set_num (par, leaf);
    size_t vertex_3_num = set_v3_num (par, leaf);

    if (par->csize == 2 && vertex_3_num == CHILD_NUM - 1) {
        lshift_fill (leaf_num, par);
        insert_parent_key (leaf_num, par);

        /*  right shift  */
        if (leaf_num == 0) {
            par->child[0]->child[1] = par->child[0]->child[0];
            par->child[0]->child[2] = par->child[0]->child[1];
        }

        insert_third_child (leaf_num, par, leaf);
        clear (leaf_num, par, leaf);
    } 
    else if (par->csize == 2 && vertex_3_num) {
        
        if (leaf_num == 2) {
            if (vertex_3_num == 1) 
                rotate_right (1, 2, par);
            else if (vertex_3_num == 2) {
                rotate_right (1, 2, par);
                par->child[1]->child[1] = par->child[1]->child[0];
                rotate_right (0, 1, par);
            } 
        }
        else if (leaf_num == 1) {
            if (vertex_3_num == 2) 
                rotate_left (2, 1, par);
            else if (vertex_3_num == 2) 
                rotate_right (0, 1, par);  
        }
        else if (leaf_num == 0) {
            if (vertex_3_num == 1) 
                rotate_left (1, 0, par);
            else if (vertex_3_num == 2) {
                rotate_left (1, 0, par);  
                rotate_left (2, 1, par);
            }
        }
    }
    else if (par->csize == 1) {
        if (leaf_num == 0 && vertex_3_num == 1) {
            if (leaf->child[0] == NULL) 
                leaf->child[0] = leaf->child[1];
            rotate_left (1, 0, par);
        } 
        else if (leaf_num == 1 && vertex_3_num == 0) {
            if (leaf->child[1] == NULL) 
                leaf->child[1] = leaf->child[0];
            rotate_right (0, 1, par);
        }
    }

    return par;
}

void move_par_key (size_t num, BNodePtr par) {
    switch (num) {
        case 0:
            insert_to_vertex (par->child[1], par->info[0]);
            break;
        case 1:
            insert_to_vertex (par->child[0], par->info[0]);
    }
}

BNodePtr non_null_child (BNodePtr leaf) {
    if (leaf->child[0]) 
        return leaf->child[0];
    if (leaf->child[1]) 
        return leaf->child[1];
    return NULL;
}

void move_children (size_t num, BNodePtr par, BNodePtr leaf) {
    switch (num) {
        case 0:
            par->child[1]->child[2] = par->child[1]->child[1];
            par->child[1]->child[1] = par->child[1]->child[0];
            par->child[1]->child[0] = non_null_child (leaf);
            if (par->child[1]->child[0])
                par->child[1]->child[0]->par = par->child[1];
            break;
        case 1:
            par->child[0]->child[2] = non_null_child (leaf);
            if (par->child[0]->child[2])
                par->child[0]->child[2]->par = par->child[0];
    }
}

void clear_merge (BNodePtr par, size_t num) {
    delete_from_vertex (par, par->info[0]->key);
    free_vertex (&(par->child[num]));
}

/*  par has two chidren of size 1  */
BNodePtr merge (BNodePtr *root, BNodePtr leaf) {
    BNodePtr par = leaf->par;
    if (!par)
        return NULL;
    size_t num = CHILD_NUM - 2;

    if (leaf == par->child[0]) 
        num = 0;
    else if (leaf == par->child[1]) 
        num = 1;

    move_par_key (num, par);
    move_children (num, par, leaf);
    clear_merge (par, num);
    
    if (par->par == NULL) {
        BNodePtr new_root = non_null_child (par);
        new_root->par = NULL;
        free_vertex (&par);
        *root = new_root;
        return new_root;
    }

    return par;
}

/*  DESTRUCTORS  */
void free_tree (BNodePtr root) {
    if (!root) 
        return;

    free_tree (root->child[0]);
    free_tree (root->child[1]);
    free_tree (root->child[2]);
    
    free_vertex (&root);
}



