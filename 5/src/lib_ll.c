#include "lib_ll.h"
#include "generic.h"
#include "lib_graph.h"
#include <assert.h>
#include <stdio.h>



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
    while (node && !EQ(name, node->info->name)) {
        if (prev)
            *prev = node;
        node = node->next;
    }
    return node;
}

void print_ll (NodePtr head) {
    
    NodePtr node = head;

    while (node) {
        
        print_node (node);
        print_node_weight (node);
            
        if (node->next)
            printf (" - ");
        else 
            printf (" ");
        node = node->next;
    } 
}

void print_node (NodePtr node) {

    if (!node)
        return;

    printf (("(%s, %lu) "), node->info->name, node->info->port);
        
}

void print_node_weight (NodePtr node) {

    if (!node)
        return;

    if (node->weight) {

        printf ("[");

        for (size_t i = 0; i < node->weight->ports_num; i++) {
            
            printf ("%lu", node->weight->avail_ports[i]);
            if (i != node->weight->ports_num - 1)
                printf (",");
        }

        printf ("]");
    }
    
}

int delete_from_ll (NodePtr *head, char *name, InfoPtr *info_to_delete) {

    assert (head);

    if (!(*head))
        return ERRWRG;

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

    *info_to_delete = node->weight;
    
    free_nullify (node);

    return ERRSUC;
}

/*  clear nodes and their weights  */
void free_ll (NodePtr *head, char *head_name) {

    if (!(*head))
        return;

    NodePtr *node = head;
    NodePtr next = *node;

    while (*node) {
        
        next = (*node)->next;

        if ((*node)->weight) {

            /*  every edge belongs to two vertices,
                so in first time we clear avail_ports field,
                in second time we clear edge ptr itself  */

            if ((*node)->weight->avail_ports) {
                free_nullify ((*node)->weight->avail_ports);

                /*  loop - one vertice edge */
                if (EQ((*node)->info->name, head_name))
                    free_nullify ((*node)->weight);
            }
            else 
                free_nullify ((*node)->weight);

        }

        free_nullify (*node);
        *node = next;
    } 

    *head = NULL;
}


