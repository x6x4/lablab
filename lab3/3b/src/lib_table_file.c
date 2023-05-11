#include "lib_table_file.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>

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

        ------------------          -----------------        ----------------     
        table & keyspaces           table & keyspaces        table & keyspaces
        ------------------    ->    -----------------   ->   ----------------- 
         k**00*k0*0*                k**00*k0*0*|k***k**      k***k**
        |<-------->|                |<--------------->| 
        initial_nodes_size          total_nodes_size
    */
    offset_t initial_last_node_offset = table->last_node_offset;
    offset_t first_node_offset = ks_offset(KSLIST_MAX_SZ+1);
    offset_t initial_nodes_size = initial_last_node_offset - first_node_offset;

    /*  For each ks write key and linked list after last current node.  */
    for (size_t i = 0; i < table->kslist_sz; i++) {

        /*  Print key of each ks.  */
        char *key = read_string (table->kslist[i].key, file);
        printf ("Key %lu: %s\n", i+1, key);
        /*  Write key after current last node.  */
        offset_t key_offset = table->last_node_offset;
        fseek (file, key_offset, SEEK_SET);
        fwrite (key, (strlen (key)+1), 1, file);
        table->last_node_offset += (strlen (key)+1);
        free (key);

        /*  Start from tail node.  */
        offset_t cur_offset = table->kslist[i].tail;
        offset_t cur_tail_offset = table->last_node_offset;
        node_d cur_node = {};

        /*  Iterate through keyspace.  */
        for (int j = 0; j < table->kslist[i].ks_sz; j++) {
            
            read_node (&cur_node, cur_offset, table, file);
            /*  Copy "pointer" to next node.  */
            cur_offset = cur_node.next;

            fseek (file, table->last_node_offset, SEEK_SET);
            /*  If cur_node == tail.  */
            if (j == (table->kslist[i].ks_sz - 1)) {
                table->kslist[i].tail = cur_tail_offset - initial_nodes_size;
                table->kslist[i].key = key_offset - initial_nodes_size;
                cur_node.next = cur_tail_offset - initial_nodes_size;
            }
            else 
                cur_node.next = table->last_node_offset - initial_nodes_size + NODE;

            cur_node.key = key_offset - initial_nodes_size;
            fwrite (&cur_node, sizeof cur_node, 1, file);

            table->last_node_offset += NODE;
        }
    }

    /*  Copy relevant nodes to buf.  */
    offset_t final_nodes_size = table->last_node_offset - initial_last_node_offset;
    void *buf = calloc (1, final_nodes_size);
    fseek (file, initial_last_node_offset, SEEK_SET);
    fread (buf, final_nodes_size, 1, file);
    /*  Write only relevant nodes to file.  */
    fseek (file, first_node_offset, SEEK_SET);
    ftruncate (fileno(file), first_node_offset + final_nodes_size);
    fwrite (buf, final_nodes_size, 1, file);

    free (buf);

    table->last_node_offset = first_node_offset + final_nodes_size;

    puts ("THAT'S ALL. YOU CAN SAFELY CONTINUE YOUR WORK NOW. THANK YOU.");

    table->erased_nodes_num = 0;
}

int insert_table_file (table_ram *table, char *key, int val, FILE *file) {

    fseek (file, 0, SEEK_SET);

    ks_d *ks = ks_by_key_file (table, key, file);

    if (!ks) {
        if (table->kslist_sz == table->kslist_max_sz) {
            printf ("Error: run out of keyspaces\n");
            return ERRWRG;
        }

        ks = new_keyspace_file (table, key, file);
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

    new_node_file (table, ks, val, file);

    table->sz++;

    return ERRSUC;
}

char *read_string (offset_t str_offset, FILE *file) {
    char c = 0;
    size_t strlen = 0;

    fseek (file, str_offset, SEEK_SET);
    do {
        fread (&c, sizeof c, 1, file);
        strlen++;
    }
    while (c != 0);

    char *key = calloc (strlen, sizeof *key);

    fseek (file, str_offset, SEEK_SET);
    fread (key, strlen, 1, file);

    return key;
} 

void print_table_file (table_ram *table, FILE *file) {

    if (!table) {
        return;
    }

    for (size_t i = 0; i < table->kslist_sz; i++) {
        
        char *key = read_string (table->kslist[i].key, file);

        printf ("Key %lu: %s\n", i+1, key);
        print_by_key (table, key, file);
        free (key);
    }
}

void print_by_key (table_ram *table, char *key, FILE *file) {

    ks_d *ks = ks_by_key_file (table, key, file);

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

    ks_d *ks = ks_by_key_file (table, key, file);

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

ks_d *ks_by_key_file (table_ram *table, char *key, FILE *file) {
    for (size_t i = 0; i < table->kslist_sz; i++) {

        char *ks_key = read_string (table->kslist[i].key, file);

        if (!strcmp (ks_key, key)) {
            free (ks_key);
            return &(table->kslist[i]);
        }
        free (ks_key);
    }

    return NULL;
}

int erase_from_table_by_key_ver_file (table_ram *table, char *key, size_t ver, FILE *file) {

    printf ("UNTIL GARBAGE COLLECTION: %d\n", ERASED_NODES_THRESHOLD - table->erased_nodes_num);

    if (table->erased_nodes_num >= ERASED_NODES_THRESHOLD) {
        garbage_collector (table, file);
        return ERRWRG;
    }

    ks_d *ks = ks_by_key_file (table, key, file);

    if (!ks) {
        puts ("Key not found");
        return ERRWRG;
    } 

    if (ks->ks_sz == 1) {
        offset_t cur_offset = ks->tail;
        node_d cur_node = {};
        read_node (&cur_node, cur_offset, table, file);
        if (cur_node.ver == ver) {
            return erase_from_table_by_key_file (table, key, file);
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

    if (done) {
        table->erased_nodes_num++;
        return ERRSUC;
    }
    else 
    {
        puts ("Item not found");
        return ERRWRG;
    }
}

int erase_from_table_by_key_file (table_ram *table, char *key, FILE *file) {
    
    printf ("UNTIL GARBAGE COLLECTION: %d\n", ERASED_NODES_THRESHOLD - table->erased_nodes_num);

    if (table->erased_nodes_num >= ERASED_NODES_THRESHOLD) {
        garbage_collector (table, file);
        return ERRWRG;
    }

    ks_d *ks = ks_by_key_file (table, key, file);

    if (!ks) {
        puts ("Key not found");
        return ERRWRG;
    }

    table->sz -= ks->ks_sz;
    size_t num = ks->num_in_table;
    
    if (num != table->kslist_sz) {
        memcpy (&(table->kslist[ks->num_in_table - 1]),
        &(table->kslist[--table->kslist_sz]), sizeof (ks_d));
        ks->num_in_table = num;
    } else 
        --table->kslist_sz;

    table->erased_nodes_num+= ks->ks_sz;
    return ERRSUC;
}

void free_table_file (table_ram *table) {
    free (table->kslist);
    free (table);
}


/*  Constructors.  */

table_ram *init_table_file (size_t kslist_max_sz) {

    table_ram *table = calloc (1, sizeof *table);

    table->sz = 0;
    table->kslist_sz = 0;

    table->kslist_max_sz = kslist_max_sz;
    table->last_node_offset = ks_offset(KSLIST_MAX_SZ+1);
    table->erased_nodes_num = 0;
    table->kslist = calloc (kslist_max_sz, sizeof *(table->kslist));

    return table;
}

ks_d *new_keyspace_file (table_ram *table, char *key, FILE *file) {

    ks_d *new_ks = &(table->kslist[table->kslist_sz]);
    new_ks->num_in_table = ++table->kslist_sz;
    new_ks->ks_sz = 0;
    new_ks->key = table->last_node_offset;
    fseek (file, new_ks->key, SEEK_SET);
    fwrite (key, (strlen(key) + 1), 1, file);

    table->last_node_offset += (strlen(key) + 1);
    new_ks->tail = table->last_node_offset;

    return new_ks;
}

void new_node_file (table_ram *table, ks_d *ks, int val, FILE *file) {

    node_d node = {};
    size_t node_offset = table->last_node_offset;

    node.key = ks->key;
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

    /*  Skip kslist field.  */
    fwrite (table, (sizeof *table - sizeof (ks_d*)), 1, file);

    fseek (file, 8, SEEK_CUR);

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
    fread (table, (sizeof *table - sizeof (ks_d*)), 1, file);

    fseek (file, 8, SEEK_CUR);

    /*  Read keyspaces.  */
    for (size_t i = 0; i < table->kslist_sz; i++) {
        fread (&(table->kslist[i]), sizeof table->kslist[i], \
               1, file);
    }

    return ERRSUC;
}

