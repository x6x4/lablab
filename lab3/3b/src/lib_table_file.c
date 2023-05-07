#include "lib_table_file.h"
#include <stdio.h>
#include <string.h>

enum {
	ERREOF = -1,
	ERRSUC,
	ERRWRG
};

void garbage_collector (table_ram *table, FILE *file) {
    
    puts ("WARNING: GARBAGE COLLECTION. PLEASE DON'T TAKE ANY ACTION TILL IT'S OVER.");

    if (!table) {
        return;
    }

    /*  
        First we copy all nodes after last node, 
        then we copy them after last keyspace.  
    */
    size_t initial_last_node_offset = table->last_node_offset;
    size_t first_node_offset = ks_offset(KSLIST_MAX_SZ+1);
    offset_t nodes_shift = initial_last_node_offset - first_node_offset;


    for (size_t i = 0; i < table->kslist_sz; i++) {
        printf ("Key %lu: %s\n", i+1, table->kslist[i].key);

        offset_t cur_offset = table->kslist[i].tail;
        node_d cur_node = {};
        read_node (&cur_node, cur_offset, table, file);
        cur_offset = cur_node.next;

        for (int j = 0; j < table->kslist[i].ks_sz; j++) {
            read_node (&cur_node, cur_offset, table, file);
            /*  Copy "pointer" to next node.  */
            cur_offset = cur_node.next;

            fseek (file, table->last_node_offset, SEEK_SET);
            cur_node.next = table->last_node_offset - nodes_shift + NODE;
            fwrite (&cur_node, sizeof cur_node, 1, file);

            table->last_node_offset += NODE;
        }
    }

    node_d *buf = calloc (table->sz, sizeof *buf);
    fseek (file, initial_last_node_offset, SEEK_SET);
    fread (buf, sizeof *buf, table->sz, file);
    fseek (file, first_node_offset, SEEK_SET);
    fwrite (buf, sizeof *buf, table->sz, file);

    puts ("THAT'S ALL. YOU CAN SAFELY CONTINUE YOUR WORK NOW. THANK YOU.");
}


void print_table (table_ram *table, FILE *file) {

    if (!table) {
        return;
    }

    for (size_t i = 0; i < table->kslist_sz; i++) {
        printf ("Key %lu: %s\n", i+1, table->kslist[i].key);
        print_by_key (table, table->kslist[i].key, file);
    }
}

void print_by_key (table_ram *table, char *key, FILE *file) {

    ks_t *ks = ks_by_key (table, key);

    if (!ks) {
        printf ("Key not found.\n");
        return;
    }

    offset_t cur_offset = ks->tail;
    node_d cur_node = {};
    read_node (&cur_node, cur_offset, table, file);
    cur_offset = cur_node.next;

    for (int i = 0; i < ks->ks_sz; i++) {
        read_node (&cur_node, cur_offset, table, file);
        
        printf ("(v%d) %d ", cur_node.ver, cur_node.val);
        if (ks->ks_sz > 1) {
            node_d next_node = {};
            read_node (&next_node, cur_node.next, table, file);
            printf ("-> %d", next_node.val);
        }
        printf ("\n");

        cur_offset = cur_node.next;
    }
}

void print_by_key_ver (table_ram *table, char *key, size_t ver, FILE *file) {

    ks_t *ks = ks_by_key (table, key);

    if (!ks) {
        printf ("Key not found.\n");
        return;
    }

    offset_t cur_offset = ks->tail;
    node_d cur_node = {};
    read_node (&cur_node, cur_offset, table, file);
    cur_offset = cur_node.next;

    for (int i = 0; i < ks->ks_sz; i++) {
        read_node (&cur_node, cur_offset, table, file);
        
        if (cur_node.ver == ver) {
            printf ("%d ", cur_node.val);

            if (ks->ks_sz > 1) {
                node_d next_node = {};
                read_node (&next_node, cur_node.next, table, file);
                printf ("-> %d", next_node.val);
            }

            printf ("\n");
            return;
        }

        cur_offset = cur_node.next;
    }

    printf ("Item not found.\n");
    return;
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

    /*if (table->sz == ANCIENT_EVIL_AWAKEN) {
        garbage_collector (table, file);
        return ERRWRG;
    }*/

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
    else {
        offset_t cur_offset = ks->tail;
        node_d cur_node = {};
        read_node (&cur_node, cur_offset, table, file);
        cur_offset = cur_node.next;

        for (int i = 0; i < ks->ks_sz; i++) {
            read_node (&cur_node, cur_offset, table, file);
            
            if (cur_node.val == val) {
                puts ("Error: repeated value.");
                return ERRWRG;
            }

            cur_offset = cur_node.next;
        }
    }

    new_node (table, ks, val, file);

    table->sz++;

    return ERRSUC;
}

int erase_from_table_by_key_ver (table_ram *table, char *key, size_t ver, FILE *file) {

    ks_t *ks = ks_by_key (table, key);

    if (!ks) {
        puts ("Key not found");
        return ERRWRG;
    } 

    if (ks->ks_sz == 1) {
        offset_t cur_offset = ks->tail;
        node_d cur_node = {};
        read_node (&cur_node, cur_offset, table, file);
        if (cur_node.ver == ver) {
            return erase_from_table_by_key (table, key);
        }
        else {
            puts ("Item not found");
            return ERRWRG;
        }
    }

    offset_t cur_offset = ks->tail;
    node_d cur_node = {};
    node_d next_node = {};
    int done = 0;

    for (int i = 0; i < ks->ks_sz; i++) {
        read_node (&cur_node, cur_offset, table, file);
        read_node (&next_node, cur_node.next, table, file);
        
        if (next_node.ver == ver) {
            done++;
            if (cur_node.next == ks->tail) 
                ks->tail = cur_offset;

            cur_node.next = next_node.next;
            fseek (file, cur_offset, SEEK_SET);
            fwrite (&cur_node, sizeof cur_node, 1, file);
            
            ks->ks_sz--;
            table->sz--;
            break;
        }

        cur_offset = cur_node.next;
    }

    if (done)
        return ERRSUC;
    else 
    {
        puts ("Item not found");
        return ERRWRG;
    }
}

int erase_from_table_by_key (table_ram *table, char *key) {
    
    ks_t *ks = ks_by_key (table, key);

    if (!ks) {
        puts ("Key not found");
        return ERRWRG;
    }

    table->sz -= ks->ks_sz;
    size_t num = ks->num;
    
    if (num != table->kslist_sz) {
        memcpy (&(table->kslist[ks->num - 1]),
        &(table->kslist[--table->kslist_sz]), sizeof (ks_t));
        ks->num = num;
    } else 
        --table->kslist_sz;

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

