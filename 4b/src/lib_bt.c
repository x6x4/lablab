#include "lib_bt.h"
#include "generic.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>
/*  LIST  */

int branch_ext (InfoListPtr *info, Key key, Val val) {

    if (!(*info))
        *info = new_infolist (key);
    
    assert(EQ(key, (*info)->key));

    InfoPtr head = (*info)->head;
    InfoPtr prev = NULL;
    
    InfoPtr node = find_in_ll_by_val (head, val, &prev);
    if (node)
        return ERRDUP;
    
    insert_to_ll (&head, val, prev); // prev here is end of list. 
    return ERRSUC;
}

void insert_to_ll (InfoPtr *head, Val val, InfoPtr prev) {
    assert(!(*head == NULL && prev != NULL)); // Check if *head is NULL then prev is NULL too.

    InfoPtr node = calloc (1, sizeof *node);
    assert(node);

    node->val = strdup (val); 
    node->ver = 0;
    node->next = prev ? prev->next : *head;
    
    assert(node->val);
    
    if (prev) {
        prev->next = node;
        node->ver = prev->ver + 1;
    } else { /* insertion to head */
        *head = node;
    }

}

InfoPtr find_in_ll_by_val (InfoPtr head, Val val, InfoPtr *prev) {
    if (!head){
        if (prev) *prev = NULL;
        return NULL;
    }

    InfoPtr node = head;
    while (node && !(EQ (node->val, val))) {
        if(prev) *prev = node;
        node = node->next;
    }
    return node;
}

InfoPtr find_in_ll_by_ver (InfoPtr head, size_t ver, InfoPtr *prev) {
    if (!head) {
        if (prev) *prev = NULL;
        return NULL;
    }
    
    InfoPtr node = head;
    while (node && node->ver != ver) {
        if(prev) *prev = node;
        node = node->next;
    }
    return node;
}

void print_ll (InfoPtr head) {
    InfoPtr node = head;

    while (node) {
        printf (YELLOW("(%lu, %s)"), node->ver, node->val);
        if (node->next)
            printf ("->");
        else 
            printf (" ");
        node = node->next;
    } 
}

int delete_from_ll (InfoPtr *head, size_t ver) {
    InfoPtr prev = NULL;
    InfoPtr node = find_in_ll_by_ver (*head, ver, &prev);

    if (!node)
        return ERRWRG;
    
    if (prev)
        prev->next = node->next;

    if (node == *head)
        //  set list head to null, if "node" is the only node in the list
        *head = node->next;

    //  free memory
    free_nullify (node->val);
    free_nullify (node);

    return ERRSUC;
}

void free_ll (InfoPtr *head) {

    InfoPtr node = *head;
    InfoPtr next = node;

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

BNodePtr new_vertex (InfoListPtr info) {
    BNodePtr node = calloc (1, sizeof *node);
    
    if(!node) 
        return NULL;
    
    node->csize = 1;
    node->info[0] = info;
    
    for (size_t i = 0; i < CHILD_NUM; i++)
        node->child[i] = NULL;
    node->par = NULL;

    return node;
}

BNodePtr new_bt_node (InfoListPtr info, BNodePtr children[4], BNodePtr par) {
    BNodePtr node = calloc (1, sizeof *node);
    
    if(!node)
        return NULL;
    
    node->csize = 1;
    node->info[0] = info;
    for (size_t i = 0; i < CHILD_NUM; i++)
        node->child[i] = children[i];
    node->par = par;

    return node;
}

InfoListPtr new_infolist (Key key) {
    InfoListPtr info = calloc (1, sizeof *info);
    
    if(!info)
        return NULL;

    info->key   = strdup (key);
    info->csize = 0;
    info->head  = NULL;

    if(!info->key) {
        free_nullify(info);
        return NULL;
    }

    return info;
}

/*  INSERTION  */ 
void insert_to_vertex (BNodePtr node, InfoListPtr info) {
    node->info[node->csize++] = info;
    sort_node (node);
}

/*  DESTRUCTORS  */

void free_vertex (BNodePtr *node) {
    for (size_t i = 0; i < (*node)->csize; i++) {
        free_infolist (&((*node)->info[i]));
    }
    free_nullify (*node);
}

void free_infolist (InfoListPtr *info) {
    if (!(*info))
        return;
    free_nullify ((*info)->key);
    free_ll (&((*info)->head));
    free_nullify ((*info));
    return;
}

/*  DELETION  */
int shift_infolists_and_change_sz (BNodePtr node, Key key) {
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

/*  SEARCH  */

int find_in_vertex (BNodePtr node, char *key, size_t *pos) {

    if (!node) 
        return ERRWRG;

    if (!key)
        return ERRWRG;

    for (size_t i = 0; i < node->csize; i++) {
        if (node->info[i] && EQ(node->info[i]->key, key)) {
            *pos = i;
            return ERRSUC;
        }
    }

    return ERRWRG;
}


/*  REORDER  */

void swap (InfoListPtr *a, InfoListPtr *b) {
    InfoListPtr buf = *a;
    *a = *b;
    *b = buf;
}

//  Result: a < b
void asc_sort_2 (InfoListPtr *a, InfoListPtr *b) {
    if ((*a) && (*b) && GT((*a)->key, (*b)->key))
        swap (a, b);
}

//  Result: a < b < c
void asc_sort_3 (InfoListPtr *a, InfoListPtr *b, InfoListPtr *c) {
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


/*  OTHER  */
void construct_root_after_split (BNodePtr root, InfoListPtr root_info, BNodePtr left, BNodePtr right) {
    root->info[0] = root_info;

    root->child[0] = left;
    root->child[1] = right;

    root->child[2] = root->child[3] = root->par = NULL;
    
    root->csize = 1;
}

Bool is_leaf (BNodePtr node) {
    return !(node->child[0] || node->child[1] || node->child[2]);
}