#include "lib_bintree.h"
#include "../../../new_input/generic.h"
#include <stddef.h>
#include <stdio.h>


/*  TREE  */

BNodePtr insert_to_tree (BNodePtr *root, BNodePtr cnode, InfoPtr info) {
    if (!(*root)) {
        *root = new_vertex (info);
        return *root;
    }

    if (is_leaf (cnode)) 
        insert_to_vertex (cnode, info);

    else if ( LTE (info->key, cnode->info[0]->key) ) 
        cnode = insert_to_tree (root, cnode->child[0], info);

    else if (cnode->csize == 1 ||
            cnode->csize == 2 && LTE (info->key, cnode->info[1]->key) ) {
        cnode = insert_to_tree (root, cnode->child[1], info);
    }
    else
        cnode = insert_to_tree (root, cnode->child[2], info);

    return split_vertex (root, cnode);
}

BNodePtr split_vertex (BNodePtr *root, BNodePtr node) {
    if (node->csize < 3)
        return node;

    BNodePtr left = NULL, right = NULL;
    create_at_split (node, &left, &right);
    
    if (node == *root) {
        left->par = *root;
        right->par = *root;
        node_to_2vertex (*root, (*root)->info[1], left, right);
        return *root;
    }
    else {
        insert_to_vertex (node->par, node->info[1]);
        
        for (size_t i = 0; i < CHILD_NUM - 1; i++) {
            if (node->par->child[i] == node) {
                node->par->child[i] = NULL;
                break;
            }
        }
        
        for (size_t i = 0; i < 3; i++) {
            if (!(node->par->child[i])) {
                node->par->child[3] = node->par->child[2];
                node->par->child[2] = node->par->child[1];

                node->par->child[i] = left;
                node->par->child[i+1] = right;    
                break;
            }
        }

        BNodePtr par = node->par;
        free_nullify (node);
        return par;
    }
}

void create_at_split (BNodePtr node, BNodePtr *left, BNodePtr *right) {
    BNodePtr left_children [4] = {node->child[0], node->child[1], NULL, NULL};
    *left = new_linked_vertex (node->info[0], left_children, node->par);
    
    BNodePtr right_children [4] = {node->child[2], node->child[3], NULL, NULL};
    *right = new_linked_vertex (node->info[2], right_children, node->par);

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

BNodePtr new_linked_vertex (InfoPtr info, BNodePtr children[4], BNodePtr par) {
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

int search_in_node (BNodePtr node, char *key, size_t *pos) {

    for (int i = 0; node->info[i]; i++) {
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
int delete_from_node (BNodePtr node, Key key) {
    switch (node->csize) {
        case 0:
            return ERRWRG;

        case 1:
            if (EQ(node->info[0]->key, key)) {
                node->info[0] = node->info[1];
                node->info[1] = node->info[2];
                node->csize--;
                return ERRSUC;
            }
        break;

        case 2:
            if (EQ(node->info[1]->key, key)) {
                node->info[1] = node->info[2];
                node->csize--;
                return ERRSUC;
            }
        break;
    }

    return ERRWRG;
}

/*  OTHER  */
void node_to_2vertex (BNodePtr node, InfoPtr info, BNodePtr node1, BNodePtr node2) {
    node->info[0] = info;
    node->child[0] = node1;
    node->child[1] = node2;
    node->child[2] = node->child[3] = node->par = NULL;
    node->csize = 1;
}

Bool is_leaf (BNodePtr node) {
    return !(node->child[0] || node->child[1] || node->child[2]);
}

