#include "../lib/auxlib.h"
#include "lib_table_ram.h"

#include <stdio.h>
#include <string.h>


table_t *init_table (size_t max_sz, size_t klist_max_sz) {

    table_t *table = calloc (1, sizeof *table);

    table->max_sz = max_sz;
    table->sz = 0;

    table->kslist_max_sz = klist_max_sz;
    table->kslist = calloc (klist_max_sz, sizeof *(table->kslist));
    table->kslist_sz = 0;

    return table;
}

int insert_table (table_t *table, char *key, int val) {
    
    if (table->sz == table->max_sz) {
        printf ("Table is full!\n");
        return ERRWRG;
    }

    ks_t *ks = ks_by_key (table, key);

    if (!ks) {
        ks = new_keyspace (table, key);
        table->kslist[table->kslist_sz - 1] = ks;
    }

    new_node (table, ks, val);

    table->sz++;

    return ERRSUC;
}

void print_table (table_t *table) {

    if (!table) {
        return;
    }
    
    printf ("Ver:  Next:      Value:     Key:\n");

    for (int i = 0; i < table->kslist_sz; i++) {

        if (table->kslist[i] != NULL) {

            node_t *cur_node = table->kslist[i]->tail->next;

            for (size_t j = 0; j < table->kslist[i]->ks_sz; j++) {

                printf ("%-5d ", cur_node->ver);

                if (cur_node->ver) {
                    printf ("%-10d ", cur_node->next->info->val);
                }
                else {
                    printf ("           ");
                }

                printf ("%-10d %s\n", cur_node->info->val, cur_node->info->key);

                cur_node = cur_node->next;
            }
        }
    }

}


ks_t *ks_by_key (table_t *table, char *key) {
    for (size_t i = 0; i < table->kslist_sz; i++) {
        if (table->kslist[i] && table->kslist[i]->key == key) {
            return table->kslist[i];
        }
    }

    return NULL;
}

table_t *node_by_key_ver (table_t *table, char *key, int ver) {

    ks_t *ks = ks_by_key (table, key);

    if (!ks) {
        return NULL;
    }

    table_t *key_table = init_table (1, 1);

    node_t *node = ks->tail->next;

    for (int i = 0; i < ks->ks_sz; i++) {

        if (node->ver == ver) {
            insert_table (key_table, key, node->info->val);
            return key_table;
        }

        node = node->next;
    }

    return NULL;
}

table_t *nodes_by_key (table_t *table, char *key) {

    ks_t *ks = ks_by_key (table, key);

    if (!ks) {
        return NULL;
    }

    table_t *key_table = init_table (ks->ks_sz, ks->ks_sz);

    node_t *node = ks->tail->next;

    for (int i = 0; i < key_table->max_sz; i++) {
        insert_table (key_table, key, node->info->val);
        node = node->next;
    }

    return key_table;
}


ks_t *new_keyspace (table_t *table, char *key) {

    ks_t *new_ks = calloc (1, sizeof *new_ks);
    new_ks->key = key;
    new_ks->tail = NULL;
    new_ks->ks_sz = 0;
    new_ks->num = ++table->kslist_sz;

    return new_ks;
}

void new_node (table_t *table, ks_t *ks, int val) {
    
    node_t *node = calloc (1, sizeof *node);

    node->info = new_info (ks->key, val);

    if (ks->tail) {
        node_t *tail = ks->tail;

        node->next = tail->next;
        tail->next = node;
        node->ver = tail->ver + 1;
        ks->tail = node;

    } else {
        ks->tail = node;
        node->next = ks->tail;
        node->ver = 1;
    }
    
    ks->ks_sz++;
    
    return;
}

info_t *new_info (char *key, int val) {
    info_t *info = calloc (1, sizeof *info); 
    info->val = val;
    info->key = key;
    return info;
}


void free_table (table_t *table) {

    for (int i = 0; i < table->kslist_sz; i++) {
        if (table->kslist[i]) {
            erase_from_table_by_key (table, table->kslist[i]->key);
        }
    }

    free (table->kslist);
    free (table);
}

int erase_ks_from_table (table_t *table, ks_t *ks) {
    
    if (!ks) {
        return ERRWRG;
    }

    for (size_t i = 0; i < ks->ks_sz; i++) {
        node_t *tail = ks->tail;
        free (tail->info);
        ks->tail = ks->tail->next;
        free (tail);
    }

    table->sz -= ks->ks_sz;
    
    if (ks->num != ks->ks_sz) {
        table->kslist[ks->num - 1] = table->kslist[--ks->ks_sz];
    } 

    free (ks);

    --table->kslist_sz;

    return ERRSUC;
}

int erase_from_table_by_key (table_t *table, char *key) {
    
    ks_t *ks = ks_by_key (table, key);

    return erase_ks_from_table (table, ks); 
}

int erase_from_table_by_key_ver (table_t *table, char *key, size_t ver) {

    ks_t *ks = ks_by_key (table, key);

    if (!ks) {
        return ERRWRG;
    } 

    if (ks->ks_sz == 1) {
        if (ver == 1) {
            return erase_ks_from_table (table, ks);
        }
        else {
            return ERRWRG;
        }
    }

    node_t *node = ks->tail->next;
    node_t *prev_node = ks->tail;

    for (int i = 0; i < ks->ks_sz; i++) {

        if (prev_node->next->ver == ver) {
            prev_node->next = node->next;
            free (node->info);
            free (node);
            node = NULL;

            ks->ks_sz--;
        } else if (prev_node->ver > ver) {
            prev_node->ver--;
        }

        prev_node = prev_node->next;

        if (node) {
            node = node->next;
        }
    }

    ks->tail = prev_node;

    return ERRSUC;
}
