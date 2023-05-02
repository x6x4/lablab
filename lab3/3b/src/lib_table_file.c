#include "lib_table.h"
#include <stdio.h>


int write_table_descriptor (table_t *table, FILE *file) {
    
    if (!file) 
        return ERRWRG;
    
    size_t format = 0xda7a;
    fseek (file, 0, SEEK_SET);
    fwrite (&format, 2, 1, file);

    size_t table_descriptor [5] = {0, 0, 0, 0, TABLE};

    table_descriptor [0] = table->max_sz;
    table_descriptor [1] = table->sz;
    table_descriptor [2] = table->kslist_max_sz;
    table_descriptor [3] = table->kslist_sz;

    fwrite (table_descriptor, sizeof (size_t), 5, file);

    return ERRSUC;
}

int read_table_descriptor (table_t *table, FILE *file) {
    
    if (!file) 
        return ERRWRG;
    
    fseek (file, 0, SEEK_SET);

    size_t table_descriptor [5] = {0, 0, 0, 0, TABLE};

    fread (table_descriptor, sizeof (size_t), 5, file);

    table->max_sz = table_descriptor [0];
    table->sz = table_descriptor [1];
    table->kslist_max_sz = table_descriptor [2];
    table->kslist_sz = table_descriptor [3];

    table->kslist = calloc (table->kslist_max_sz, sizeof *(table->kslist));

    return ERRSUC;
}

table_t *init_table (size_t max_sz, size_t kslist_max_sz) {

    table_t *table = calloc (1, sizeof *table);

    table->max_sz = max_sz;
    table->sz = 0;

    table->kslist_max_sz = kslist_max_sz;
    table->kslist = calloc (kslist_max_sz, sizeof *(table->kslist));
    table->kslist_sz = 0;

    return table;
}

int write_ks_descriptor (FILE *file, table_t *table, ks_t *ks) {
    
    if (!file) 
        return ERRWRG;

    ks_d ks_descriptor = {0, 0, 0, NULL};

    ks_descriptor.num = ks->num;
    ks_descriptor.ks_sz = ks->ks_sz;
    ks_descriptor.key = ks->key;

    fwrite (&ks_descriptor, sizeof (ks_descriptor), 1, file);

    return ERRSUC;
}

ks_t *new_keyspace (table_t *table, char *key) {

    ks_t *new_ks = calloc (1, sizeof *new_ks);
    new_ks->key = key;
    new_ks->tail = NULL;
    new_ks->ks_sz = 0;
    new_ks->num = ++table->kslist_sz;

    return new_ks;
}

int insert_table (table_t *table, char *key, int val) {
    
    if (table->sz == table->max_sz) {
        printf ("Table is full!\n");
        return ERRWRG;
    }

    ks_t *ks = ks_by_key (table, key);

    if (!ks) {
        ks = new_keyspace (table, key);
        size_t new_ks_offset = ks_offset((table->kslist_sz - 1));
        //fwrite ()
        table->kslist[table->kslist_sz - 1] = ks;
    }

    new_node (table, ks, val);

    table->sz++;

    return ERRSUC;
}
