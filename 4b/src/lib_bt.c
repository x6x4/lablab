#include "lib_bt.h"
#include "generic.h"
#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>



/*||||||||||||||||||||||||| < LIST > |||||||||||||||||||||||||*/


void branch_ext (InfoListPtr *info, Key key, Val val) {

    if (!(*info))
        *info = new_infolist (key);

    assert (*info != NULL);
    assert (EQ((*info)->key, key));

    InfoPtr *head = &((*info)->head);

    //  insert to the start of the list 
    insert_to_ll (head, val, NULL);    
}

void insert_to_ll (InfoPtr *head, Val val, InfoPtr prev) {

    //  null head value should entail null prev value
    assert (*head || (*head == NULL && prev == NULL));

    InfoPtr node = calloc (1, sizeof *node);
    assert (node);

    node->val = strdup (val); 
    assert (node->val);

    node->ver = 0;
    
    if (prev) {
        node->next = prev->next;
        prev->next = node;
        node->ver = prev->ver + 1;
    } else {
        //  insert to start (make node a new head)
        node->next = *head;
        *head = node;
    }

}

InfoPtr find_in_ll_by_ver (InfoPtr head, size_t ver, InfoPtr *prev) {
    
    if (!head) {
        //  set prev according to head
        if (prev)
            *prev = NULL;

        return NULL; 
    }

    InfoPtr node = head;
    while (node && node->ver != ver) {
        if (prev)
            *prev = node;
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

    //  prev is either null or previous for the node
    assert (!prev || prev->next == node);

    if (node == *head)
        *head = node->next;
    else 
        prev->next = node->next;        

    free_nullify (node->val);
    free_nullify (node);

    return ERRSUC;
}

void free_ll (InfoPtr *head) {

    if (!(*head))
        return;

    InfoPtr *node = head;
    InfoPtr next = *node;

    while (*node) {
        next = (*node)->next;
        free_nullify ((*node)->val);
        free_nullify (*node);
        *node = next;
    } 

    *head = NULL;
}



/*||||||||||||||||||||||||| < NODE > |||||||||||||||||||||||||*/


/*========================= < CONSTRUCTORS > =========================*/


BNodePtr new_vertex (InfoListPtr info) {
    BNodePtr node = calloc (1, sizeof *node);
    assert (node);

    node->csize = 1;
    node->info[0] = info;
    
    for (size_t i = 0; i < CHILD_NUM; i++)
        node->child[i] = NULL;
    node->par = NULL;

    return node;
}

BNodePtr new_bt_node (InfoListPtr info, BNodePtr children[4], BNodePtr par) {
    BNodePtr node = calloc (1, sizeof *node);
    assert (node);

    node->csize = 1;
    node->info[0] = info;
    for (size_t i = 0; i < CHILD_NUM; i++)
        node->child[i] = children[i];
    node->par = par;

    return node;
}

InfoListPtr new_infolist (Key key) {
    InfoListPtr info = calloc (1, sizeof *info);
    assert (info);

    info->key = strdup (key);
    assert (info->key);

    info->csize = 0;
    info->head = NULL;

    return info;
}

void new_root_from_fields (BNodePtr root, InfoListPtr root_info, BNodePtr left, BNodePtr right) {
    assert (root);
    assert (root_info);
    assert (left);
    assert (right);
    
    root->info[0] = root_info;

    root->child[0] = left;
    root->child[1] = right;

    root->child[2] = root->child[3] = root->par = NULL;
    
    root->csize = 1;
}


/*========================= < INSERTION > =========================*/


void insert_to_vertex (BNodePtr node, InfoListPtr info) {
    assert (node);
    assert (node->csize < KEYS_NUM);

    node->info[node->csize++] = info;
    sort_node (node);
}


/*========================= < DESTRUCTORS > =========================*/


void free_vertex (BNodePtr *node) {
    for (size_t i = 0; i < (*node)->csize; i++) {
        free_infolist (&((*node)->info[i]));
    }
    free_nullify (*node);
}

void free_infolist (InfoListPtr *info) {
    if ((!info) || !(*info))
        return;

    free_nullify ((*info)->key);
    free_ll (&((*info)->head));
    free_nullify (*info);
    return;
}


/*========================= < DELETION > =========================*/


int shift_infolists_and_change_sz (BNodePtr node, Key key) {
    assert (node);
    assert (key);

    if (node->csize == 0) 
        return ERRWRG;

    for (size_t i = 0; i < node->csize; i++) {

        //  we need key when we move infos without cleanup
        if (node->info[i] == NULL || EQ(node->info[i]->key, key) ) {
            
            /*  Left shift.  */
            for (size_t j = i; j < node->csize - 1; j++) 
                node->info[j] = node->info[j+1];

            node->csize--;
            return ERRSUC;
        }
    }

    return ERRWRG;
}


/*========================= < SEARCH > =========================*/


int find_in_vertex (BNodePtr node, char *key, size_t *pos) {

    assert (pos);

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


/*========================= < REORDER > =========================*/


void swap (InfoListPtr *a, InfoListPtr *b) {
    assert (a);
    assert (b);

    InfoListPtr buf = *a;
    *a = *b;
    *b = buf;
}

//  Result: a < b
void asc_sort_2 (InfoListPtr *a, InfoListPtr *b) {
    assert (a);
    assert (b);

    if ((*a) && (*b) && GT((*a)->key, (*b)->key))
        swap (a, b);
}

//  Result: a < b < c
void asc_sort_3 (InfoListPtr *a, InfoListPtr *b, InfoListPtr *c) {
    assert (a);
    assert (b);
    assert (c);

    if (GT((*a)->key, (*b)->key))
        swap (a, b);
    if (GT((*a)->key, (*c)->key))
        swap (a, c);
    if (GT((*b)->key, (*c)->key))
        swap (b, c);
}

void sort_node (BNodePtr node) {
    assert (node);

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

        default:
            assert (0 && "BAD NODE");
    }
}



/*||||||||||||||||||||||||| < VALIDATION > |||||||||||||||||||||||||*/


const char* is_valid_node(const BNodePtr node) {
    /*  no node  */
    if (node == NULL) 
        return NULL;

    try (check_sz (node));
    try (check_node_infos (node));
    try (check_node_children(node));

    return NULL;
}


static const char* is_valid_info(const InfoListPtr info) {
    /*  check info  */
    if (info == NULL) 
        return "Info is NULL";

    /*  check key  */
    else if (info->key == NULL) 
        return "EMPTY KEY";

    /*  all is ok  */
    else 
        return NULL;
}

const char* check_sz (BNodePtr node) {
    if (node->csize > KEYS_NUM) 
        return "TOO big csize";
    else
        return NULL;
}

const char* check_node_infos (BNodePtr node) {
    for(size_t i = 0; i < node->csize; ++i) {
        /*  check info  */
        try (is_valid_info(node->info[i]));

        /*  check key order  */
        if (i != 0 && GT(node->info[i-1]->key, node->info[i]->key)) 
            return "BAD KEYS ORDER";
    }
    return NULL;
}

const char* check_node_children (BNodePtr node) {
    Bool is_leaf = is_leaf(node);

    if (!is_leaf) {

        for(size_t i = 0; i < node->csize+1; ++i) {

            /*  check children  */
            if (node->child[i] == NULL) 
                return "Missing child in non-leaf node";

            /*  recursion  */
            try (is_valid_node(node->child[i]));

            /*  check parent link  */
            if (node->child[i]->par != node) 
                return "Bad parent link";

            /*  check child order  */

            if (i != 0) {
                for(size_t j = 0; j < node->child[i]->csize; ++j) {
                    if (node->child[i]->info[j] &&
                        GT(node->info[i-1]->key, node->child[i]->info[j]->key)) {
                            return "Bad child order: child key less than parent";
                        }
                }
            }

            if (i != node->csize) {
                for (size_t j = 0; j < node->child[i]->csize; ++j) {
                    if (node->child[i]->info[j] &&
                        LT(node->info[i]->key, node->child[i]->info[j]->key)) {
                            return "Bad child order: child key greater than parent";
                        }
                }
            }
        }
    } 
 
    return NULL;
}