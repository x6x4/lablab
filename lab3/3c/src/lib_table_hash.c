#include "lib_table_hash.h"

#include <stdio.h>
#include <string.h>

enum {
	ERREOF = -1,
	ERRSUC,
	ERRWRG
};

size_t hash (char *key) {
    return key[0];
}

int hs_by_hash (table_t *table, char *key) {
    for (size_t i = 0; i < table->hslist_sz; i++) {
        if (hash(table->hslist[i]->key) == hash(key)) {
            return i;
        }
    }

    return -1;
}

int insert_table (table_t *table, char *key, int val) {

    int hs_num_in_hslist = hs_by_hash (table, key);

    if (hs_num_in_hslist == -1) {
        if (table->hslist_sz == table->hslist_max_sz) {
            printf ("Error: run out of hashspaces\n");
            return ERRWRG;
        }   
    } 
    else {
        ks_t *cur_node = table->hslist[hs_num_in_hslist]->next;
        ks_t *head = cur_node;

        do {
            if (!strcmp (cur_node->key, key)) {
                printf ("Error: repeated key\n");
                return ERRWRG;
            }
            if (cur_node->info->val == val) {
                printf ("Error: repeated value\n");
                return ERRWRG;
            }
            cur_node = cur_node->next;
        } while (cur_node != head);
    }

    new_ks (table, key, val);

    return ERRSUC;
}

void print_table (table_t *table) {

    if (!table) {
        return;
    }

    for (size_t i = 0; i < table->hslist_sz; i++) {

        printf ("Hash %lu: %lu\n", i+1, hash (table->hslist[i]->key));

            ks_t *cur_node = table->hslist[i]->next;
            ks_t *head = cur_node;

            do {

                printf ("(\"%s\", %lu)", cur_node->info->key, cur_node->info->val);

                if (cur_node->next != head) {
                    printf ("-> ");
                }

                cur_node = cur_node->next;
            } while (cur_node != head);

            printf ("\n");
    }
}

table_t *nodes_by_key (table_t *table, char *key) {

    int hs_num_in_hslist = hs_by_hash (table, key);

    if (hs_num_in_hslist == -1) {
        return NULL;
    }

    table_t *key_table = init_table (1);

    ks_t *cur_node = table->hslist[hs_num_in_hslist]->next;
    ks_t *head = cur_node; 

    do { 
        if (!strcmp (cur_node->info->key, key)) {
            insert_table (key_table, key, cur_node->info->val);
            return key_table;
        }
        cur_node = cur_node->next;
    } while (cur_node != head);

    return NULL;
}

table_t *init_table (size_t klist_max_sz) {

    table_t *table = calloc (1, sizeof *table);

    table->hslist_max_sz = klist_max_sz;
    table->hslist = calloc (klist_max_sz, sizeof *(table->hslist));
    table->hslist_sz = 0;

    return table;
}

void new_ks (table_t *table, char *key, int val) {
    
    ks_t *ks = calloc (1, sizeof *ks);
    ks->key = calloc (1, strlen (key) + 1);
    strncpy (ks->key, key, strlen (key) + 1);

    ks->info = new_info (key, val);

    int hs_num_in_hslist = hs_by_hash (table, key);

    if (hs_num_in_hslist != -1) {
        ks_t *tail = table->hslist[hs_num_in_hslist];

        ks->next = tail->next;
        tail->next = ks;
        table->hslist[hs_num_in_hslist] = ks;

    } else {
        table->hslist[table->hslist_sz] = ks;
        ks->next = ks;
        table->hslist_sz++;
    }
    
    return;
}

info_t *new_info (char *key, int val) {
    info_t *info = calloc (1, sizeof *info); 
    info->val = val;
    info->key = calloc (1, strlen (key) + 1);
    strncpy (info->key, key, strlen (key) + 1);
    return info;
}


void free_table (table_t *table) {

    for (int i = table->hslist_sz - 1; i >= 0; i--) {
        ks_t *cur_node = table->hslist[i]->next;
        ks_t *head = cur_node; 
        do { 
            ks_t *next_node = cur_node->next;
            free (cur_node->info->key);
            free (cur_node->info);
            free (cur_node->key);
            free (cur_node);
            cur_node = next_node;
        } while (cur_node != head);
    }

    free (table->hslist);
    free (table);
}

int erase_from_table_by_key (table_t *table, char *key) {

    int hs_num_in_hslist = hs_by_hash (table, key);

    if (hs_num_in_hslist == -1) {
        return ERRWRG;
    }

    ks_t *cur_node = table->hslist[hs_num_in_hslist]->next;
    ks_t *head = cur_node; 

    do { 
        if (!strcmp (cur_node->next->info->key, key)) {
            ks_t *node_to_free = cur_node->next;
            if (node_to_free->next == head) {
                table->hslist[hs_num_in_hslist] = cur_node;
            }
            cur_node->next = node_to_free->next;
            free (node_to_free->info);
            free (node_to_free->key);
            free (node_to_free);
            return ERRSUC;
        }
        cur_node = cur_node->next;
    } while (cur_node != head);

    return ERRWRG;
}


