#include "lib_bintree.h"
#include "../../../new_input/generic.h"









/*  NODE  */

/*  CONSTRUCTORS  */

BNodePtr new_vertex (Key key, Key val) {
    BNodePtr node = calloc (1, sizeof *node);

    node->csize = 1;
    node->info[0] = new_info (key, val);
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
void insert_to_node (BNodePtr node, InfoPtr info) {
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
void to_2vertex (BNodePtr node, InfoPtr info, BNodePtr node1, BNodePtr node2) {
    node->info[0] = info;
    node->child[0] = node1;
    node->child[1] = node2;
    node->child[2] = node->child[3] = node->par = NULL;
    node->csize = 1;
}

Bool is_leaf (BNodePtr node) {
    return (node->child[0] && node->child[1] && node->child[2]);
}

