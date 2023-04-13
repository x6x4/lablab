#include "../../lib/auxlib.h"
#include "lib_table_ram.h"

#include <stdio.h>
#include <string.h>


table_t *init_table (size_t max_sz) {

    table_t *table = calloc (1, sizeof *table);

    table->cur_sz = 0;
    table->max_sz = max_sz;
    table->nodes = calloc (max_sz, sizeof *(table->nodes));

    return table;
}

int insert_table (table_t *table, char *key, int val) {
    
    if (table->cur_sz == table->max_sz) {
        printf ("Table is full!\n");
        return ERRWRG;
    }

    node_t *node = new_node (table, key, val);
    table->nodes[table->cur_sz++] = node;

    return ERRSUC;
}

void print_table (table_t *table) {
    
    printf ("Ver:  Next:      Value:     Key:\n");

    for (int i = 0; i < table->cur_sz; i++) {

        printf ("%-5d ", table->nodes[i]->ver);

        if (table->nodes[i]->next) {
            printf ("%-10d ", table->nodes[i]->next->info->val);
        }
        else {
            printf ("           ");
        }
        printf ("%-10d %s\n", table->nodes[i]->info->val, table->nodes[i]->info->key);
    }

}

node_t *new_node (table_t *table, char *key, int val) {
    
    node_t *node = calloc (1, sizeof *node);

    node->ver = 1;
    
    node_t *prev_node = node_by_key (table, key); 
    if (prev_node) {
        prev_node->next = node;
        node->ver = prev_node->ver + 1;
    }

    node->next = NULL;
    node->info = new_info (key, val);
    
    return node;
}

info_t *new_info (char *key, int val) {
    info_t *info = calloc (1, sizeof *info); 
    info->val = val;
    info->key = key;
    return info;
}

node_t *node_by_key (table_t *table, char *key) {
    for (int i = 0; i < table->cur_sz; i++) {
        if (0 == strcmp (table->nodes[i]->info->key, key)) {
            return table->nodes[i];
        }
    }

    return NULL;
}

void free_table (table_t *table) {
    for (int i = 0; i < table->cur_sz; i++) {
        free (table->nodes[i]->info);
        free (table->nodes[i]);
    }

    free (table->nodes);
    free (table);
}