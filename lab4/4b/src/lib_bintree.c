#include "lib_bintree.h"
#include "../../../new_input/generic.h"
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>


/*  TREE  */

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
void print_bt (BNodePtr root) {
    set_height (root);
    print_bt_lvl (root, 0);
}

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

void print_bt_lvl (BNodePtr root, size_t height) {
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
            printf ("(%s, %s) ", root->info[i]->key , root->info[i]->val);
    }
    for (size_t i = 0; i < CHILD_NUM - 1; i++) {
        null_children = 1;
        for (size_t j = 0; j < CHILD_NUM - 1; j++) {
            if (root->child[j])
                null_children = 0;
        }
        print_bt_lvl (root->child[i], height);
    }
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
        if (EQ(root->info[i]->key, key))
            printf ("\x1b[31m(%s, %s)\x1b[0m", root->info[i]->key, root->info[i]->val);
        else 
            printf ("(%s, %s) ", root->info[i]->key, root->info[i]->val);
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
    if ( root == NULL || root->child[0] == NULL /*leaf*/ )
        return root;
    for (size_t i = 0; i < CHILD_NUM - 1; i++) {
        if (root->child[i+1] == NULL) 
            return find_max (root->child[i]);
    }
    return find_max (root->child[CHILD_NUM-1]);
}

/*  DELETE  */
int delete_bt (BNodePtr *root, Key key) {
    size_t pos_node = 0, pos_max = 0;
    BNodePtr max_in_left_subtree = NULL;

    BNodePtr node_to_delete = find_bt (*root, key, &pos_node);
    if (!node_to_delete)
        return ERRWRG;

    max_in_left_subtree = find_max (node_to_delete->child[pos_node]);
    
    if (max_in_left_subtree) /*skip if leaf*/ {
        /*  swap key with max key of the left child  */
        swap (&(max_in_left_subtree->info[max_in_left_subtree->csize - 1]), 
              &(node_to_delete->info[pos_node]));
        node_to_delete = max_in_left_subtree;
    }

    /*  delete key in fact  */
    delete_from_vertex (node_to_delete, key);
    //fix_after_del (root, node_to_delete);
    return ERRSUC;
}

BNodePtr fix_after_del (BNodePtr *root, BNodePtr leaf) {
    /*  the only key in the tree  */
    if (leaf->csize == 0 && leaf->par == NULL) {
        free_nullify(leaf);
        return NULL;
    }

    /*  vertex with 2 keys  */
    if (leaf->csize != 0) {
        //if (leaf->par)
            //return fix_after_del (leaf->par);
        //else 
            return leaf;
    }

    BNodePtr par = leaf->par;
    if (par->child[0]->csize == 2 || par->child[1]->csize == 2
        || par->csize == 2) 
        ;
        //leaf = redistribute (leaf);
    else if (par->csize == 2 && par->child[2]->csize == 2)
        ;//leaf = redistribute (leaf);
    else 
        //leaf = merge (leaf);

    return fix_after_del (root, leaf);
}







/*  DESTRUCTORS  */
void free_tree (BNodePtr root) {
    if (!root) 
        return;

    free_tree (root->child[0]);
    free_tree (root->child[1]);
    free_tree (root->child[2]);
    
    free_vertex (root);
}

/*  NODE  */

/*  CONSTRUCTORS  */

BNodePtr new_vertex (InfoPtr info) {
    BNodePtr node = calloc (1, sizeof *node);

    node->csize = 1;
    node->info[0] = info;
    for (size_t i = 0; i < CHILD_NUM; i++)
        node->child[i] = NULL;
    node->par = NULL;

    return node;
}

BNodePtr new_bt_node (InfoPtr info, BNodePtr children[4], BNodePtr par) {
    BNodePtr node = calloc (1, sizeof *node);

    node->csize = 1;
    node->info[0] = info;
    for (size_t i = 0; i < CHILD_NUM; i++)
        node->child[i] = children[i];
    node->par = par;

    return node;
}

InfoPtr new_info (Key key, Key val) {
    InfoPtr info = calloc (1, sizeof *info);
    info->key = strdup (key);
    info->val = strdup (val);

    return info;
}

/*  DESTRUCTORS  */

void free_vertex (BNodePtr node) {
    for (size_t i = 0; i < node->csize; i++) {
        free_info (node->info[i]);
    }
    free_nullify (node);
}

void free_info (InfoPtr info) {
    free_nullify (info->key);
    free_nullify (info->val);
    free_nullify (info);
    return;
}

/*  SEARCH  */

int find_in_vertex (BNodePtr node, char *key, size_t *pos) {

    if (!key)
        return ERRWRG;

    for (size_t i = 0; i < node->csize; i++) {
        if (EQ(node->info[i]->key, key)) {
            *pos = i;
            return ERRSUC;
        }
    }

    return ERRWRG;
}


/*  REORDER  */

void swap (InfoPtr *a, InfoPtr *b) {
    InfoPtr buf = *a;
    *a = *b;
    *b = buf;
}

//  Result: a < b
void asc_sort_2 (InfoPtr *a, InfoPtr *b) {
    if (GT((*a)->key, (*b)->key))
        swap (a, b);
}

//  Result: a < b < c
void asc_sort_3 (InfoPtr *a, InfoPtr *b, InfoPtr *c) {
    if (GT((*a)->key, (*b)->key))
        swap (a, b);
    if (GT((*a)->key, (*c)->key))
        swap (a, c);
    if (GT((*b)->key, (*c)->key))
        swap (b, c);
}

void sort_node (BNodePtr node) {
    switch (node->csize) {
        case 0:
        case 1:
            return;

        case 2:
            asc_sort_2 (&(node->info[0]), &(node->info[1]));
            return;
        case 3:
            asc_sort_3 (&(node->info[0]), &(node->info[1]), &(node->info[2]));
            return;
    }
}

/*  INSERTION  */ 
void insert_to_vertex (BNodePtr node, InfoPtr info) {
    node->info[node->csize++] = info;
    sort_node (node);
}

/*  DELETION  */
int delete_from_vertex (BNodePtr node, Key key) {
    if (node->csize == 0) 
        return ERRWRG;

    for (size_t i = 0; i < KEYS_NUM; i++) {
        if (EQ(node->info[i]->key, key)) {
            free_info (node->info[i]);
            /*  Left shift.  */
            for (size_t j = i; j < KEYS_NUM - 1; j++) 
                node->info[j] = node->info[j+1];

            node->csize--;
            return ERRSUC;
        }
    }

    return ERRWRG;
}

/*  OTHER  */
void fix_root_after_split (BNodePtr node, InfoPtr info, BNodePtr node1, BNodePtr node2) {
    node->info[0] = info;
    node->child[0] = node1;
    node->child[1] = node2;
    node->child[2] = node->child[3] = node->par = NULL;
    node->csize = 1;
}

Bool is_leaf (BNodePtr node) {
    return !(node->child[0] || node->child[1] || node->child[2]);
}

