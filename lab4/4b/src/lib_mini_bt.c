#include "lib_mini_bt.h"
#include "generic.h"
#include <stdio.h>

/*  LIST  */

int change_info (InfoPtr *info, Key key, char *val) {

    if (!(*info))
        *info = new_info (key);

    return insert_to_ll (&((*info)->head), val);    
}

int insert_to_ll (INode *head, char *val) {
    INode prev = NULL;
    INode node = find_in_ll_by_val (val, *head, &prev);
    if (node)
        return ERRDUP;
    
    node = insert_to_ll_end (val, prev);
    /*  if end is start  */
    if (!(*head))
        *head = node;
    
    return ERRSUC;
}

INode insert_to_ll_end (char *val, INode prev) {
    INode node = calloc (1, sizeof *node);
    node->val = strdup (val); 
    node->ver = 0;
    node->next = NULL;
    
    if (prev) {
        prev->next = node;
        node->ver = prev->ver + 1;
    }
    return node;
}

INode find_in_ll_by_val (char *val, INode head, INode *prev) {
    if (!head)
        return NULL;

    INode node = head;
    while (node && !(EQ (node->val, val))) {
        *prev = node;
        node = node->next;
    }
    return node;
}

INode find_in_ll_by_ver (size_t ver, INode head, INode *prev) {
    INode node = head;
    while (node && node->ver != ver) {
        *prev = node;
        node = node->next;
    }
    return node;
}

void print_ll (INode head) {
    INode node = head;

    while (node) {
        printf (CYAN("(%s, %lu)"), node->val, node->ver);
        if (node->next)
            printf ("->");
        node = node->next;
    } 
    //printf ("\n");
}

int delete_from_ll (INode *head, size_t ver) {
    INode prev = NULL;
    INode node = find_in_ll_by_ver (ver, *head, &prev);
    if (!node)
        return ERRWRG;
    
    prev->next = node->next;
    free_nullify (node->val);
    return ERRSUC;
}

void free_ll (INode *head) {

    INode node = *head;
    INode next = node;

    while (node) {
        next = node->next;
        free_nullify (node->val);
        free_nullify (node);
        node = next;
    } 

    *head = NULL;
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

InfoPtr new_info (Key key) {
    InfoPtr info = calloc (1, sizeof *info);
    info->key = strdup (key);
    info->csize = 0;
    info->head = NULL;

    return info;
}

/*  DESTRUCTORS  */

void free_vertex (BNodePtr *node) {
    for (size_t i = 0; i < (*node)->csize; i++) {
        free_info (&((*node)->info[i]));
    }
    free_nullify (*node);
}

void free_info (InfoPtr *info) {
    if (!(*info))
        return;
    free_nullify ((*info)->key);
    free_ll (&((*info)->head));
    free_nullify ((*info));
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

    for (size_t i = 0; i < node->csize; i++) {
        if (node->info[i] == NULL || EQ(node->info[i]->key, key)) {
            /*  Left shift.  */
            for (size_t j = i; j < node->csize - 1; j++) 
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