#include "lib_ll.h"
#include "generic.h"
#include <assert.h>

void insert_to_ll (NodePtr *head, NodePtr v, NodePtr prev) {

    //  null head value should entail null prev value
    assert (*head || (*head == NULL && prev == NULL));

    NodePtr node = v;
    
    if (prev) {
        node->next = prev->next;
        prev->next = node;
    } else {
        //  insert to start (make node a new head)
        node->next = *head;

        *head = node;
    }

}

NodePtr find_in_ll (NodePtr head, char *name, NodePtr *prev) {
    
    if (!head) {
        //  set prev according to head
        if (prev)
            *prev = NULL;

        return NULL; 
    }

    NodePtr node = head;
    while (node && EQ(name, node->info->name)) {
        if (prev)
            *prev = node;
        node = node->next;
    }
    return node;
}

void print_ll (NodePtr head) {
    NodePtr node = head;

    while (node) {
        printf (YELLOW("(%s, %lu)"), node->info->name, node->info->port);
        if (node->next)
            printf ("->");
        else 
            printf (" ");
        node = node->next;
    } 
}

int delete_from_ll (NodePtr *head, char *name) {

    assert (head);

    NodePtr prev = NULL;
    NodePtr node = find_in_ll (*head, name, &prev);

    if (!node)
        return ERRWRG;

    //  prev is either null or previous for the node
    assert (!prev || prev->next == node);

    if (node == *head)
        *head = node->next;
    else 
        prev->next = node->next;        

    free_nullify (node->info->name);
    free_nullify (node->info);
    free_nullify (node);

    return ERRSUC;
}

void free_ll (NodePtr *head) {

    if (!(*head))
        return;

    NodePtr *node = head;
    NodePtr next = *node;

    while (*node) {
        next = (*node)->next;
        free_nullify ((*node)->info->name);
        free_nullify ((*node)->info);
        free_nullify (*node);
        *node = next;
    } 

    *head = NULL;
}


