#include "lib_table_file.h"
#include <stdio.h>
#include <string.h>

enum {
	ERREOF = -1,
	ERRSUC,
	ERRWRG
};


void print_table (table_ram *table, FILE *table_disk) {

    if (!table) {
        return;
    }

    for (size_t i = 0; i < table->kslist_sz; i++) {

        printf ("Key %lu: %s\n", i+1, table->kslist[i].key);

        offset_t cur_offset = table->kslist[i].tail;
        node_d cur_node = {};
        read_node (&cur_node, cur_offset, table, table_disk);
        cur_offset = cur_node.next;

        for (size_t j = 0; j < table->kslist[i].ks_sz; j++) {

            read_node (&cur_node, cur_offset, table, table_disk);

            printf ("(v%d) %d ", cur_node.ver, cur_node.val);

            if (table->kslist[i].ks_sz > 1) {
                node_d next_node = {};
                read_node (&next_node, cur_node.next, table, table_disk);
                printf ("-> %d", next_node.val);
            }
            printf ("\n");

            cur_offset = cur_node.next;
        }
    }
}

ks_t *ks_by_key (table_ram *table, char *key) {
    for (size_t i = 0; i < table->kslist_sz; i++) {
        if (!strcmp (table->kslist[i].key, key)) {
            return &(table->kslist[i]);
        }
    }
    return NULL;
}

int insert_table (table_ram *table, char *key, int val, FILE *file) {
    fseek (file, 0, SEEK_SET);

    ks_t *ks = ks_by_key (table, key);

    if (!ks) {
        if (table->kslist_sz == table->kslist_max_sz) {
            printf ("Error: run out of keyspaces\n");
            return ERRWRG;
        }

        ks = new_keyspace (table, key);
        table->kslist[table->kslist_sz - 1] = *ks;
    }

    new_node (table, ks, val, file);

    table->sz++;

    return ERRSUC;
}

void free_table (table_ram *table) {
    free (table->kslist);
    free (table);
}


/*  Constructors.  */

table_ram *init_table (size_t kslist_max_sz) {

    table_ram *table = calloc (1, sizeof *table);

    table->sz = 0;
    table->kslist_sz = 0;

    table->kslist_max_sz = kslist_max_sz;
    table->last_node_offset = ks_offset(KSLIST_MAX_SZ+1);
    table->kslist = calloc (kslist_max_sz, sizeof *(table->kslist));

    return table;
}

ks_t *new_keyspace (table_ram *table, char *key) {

    ks_t *new_ks = &(table->kslist[table->kslist_sz]);
    new_ks->num = ++table->kslist_sz;
    new_ks->ks_sz = 0;
    strncpy (new_ks->key, key, strlen (key));
    new_ks->tail = table->last_node_offset;

    return new_ks;
}

void new_node (table_ram *table, ks_t *ks, int val, FILE *file) {

    node_d node = {};
    size_t node_offset = table->last_node_offset;

    node.key = ks_key_offset(ks->num);
    node.val = val;

    /*  Make a copy of tail node of ks.  */
    node_d tail = {};
    read_node (&tail, ks->tail, table, file);
    offset_t tail_offset = ks->tail;
    
    if (tail.ver != 0) {
        node.next = tail.next;
        tail.next = node_offset;
        ks->tail = node_offset;

        /*  Write tail node back to file.  */
        fseek (file, tail_offset, SEEK_SET);
        fwrite (&tail, sizeof tail, 1, file);
    } 
    else {
        ks->tail = node_offset;
        node.next = ks->tail;
    }
    
    node.ver = tail.ver + 1;

    ks->ks_sz++;

    /*  Write current node to file.  */
    fseek (file, table->last_node_offset, SEEK_SET);
    fwrite (&node, sizeof node, 1, file);

    table->last_node_offset += NODE;
    
    return;
}

int read_node (node_d *new_node, offset_t node_offset, table_ram *table, FILE *file) {
    
    if (!file) 
        return ERRWRG;
    
    fseek (file, node_offset, SEEK_SET);

    /*  Node_d contains 4 fields, 8 bytes each:
        ver, next, key, val
    */
    fread (new_node, sizeof *new_node, 1, file);

    return ERRSUC;
}


/*  IO functions.  */

/*  write table descriptor to disk  */
int write_table_descriptor (table_ram *table, FILE *file, char *filename) {
    
    if (!file) 
        return ERRWRG;
    
    size_t format = 0xda7a;
    fseek (file, 0, SEEK_SET);
    /*  First 16 bytes of table_d.  */
    fwrite (&format, 2, 1, file); 
    fwrite (filename, strlen (filename), 1, file); 
    fseek (file, 16, SEEK_SET);

    /*  Table_d also contains 4 fields, 8 bytes each:
        sz, kslist_max_sz, kslist_sz, last_node_offset
    */
    fwrite (table, sizeof (size_t), 4, file);

    /*  Write keyspaces.  */
    for (size_t i = 0; i < table->kslist_sz; i++) {
        fwrite (&(table->kslist[i]), sizeof (table->kslist[i]), 1, file);
    }

    return ERRSUC;
}

/*  read table descriptor to main memory  */
int read_table_descriptor (table_ram *table, FILE *file) {
    
    if (!file) 
        return ERRWRG;
    
    /*  Skip format and filename.  */
    fseek (file, 0x10, SEEK_SET);

    /*  Skip kslist field.  */
    fread (table, (sizeof *table - sizeof (ks_t*)), 1, file);

    /*  Read keyspaces.  */
    for (size_t i = 0; i < table->kslist_sz; i++) {
        fread (&(table->kslist[i]), sizeof table->kslist[i], \
               1, file);
    }

    return ERRSUC;
}

