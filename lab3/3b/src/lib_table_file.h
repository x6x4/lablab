#include "lib_table.h"
#include <stdio.h>

/*  Base functions.  */



int insert_table (table_ram *table, char *key, int val, FILE *file);

void print_table (table_ram *table, FILE *table_disk);

void free_table (table_ram *table);

ks_t *ks_by_key (table_ram *table, char *key);


/*  Constructors.  */
table_ram *init_table (size_t kslist_max_sz);
ks_t *new_keyspace (table_ram *table, char *key);
void new_node (table_ram *table, ks_t *ks, int val, FILE *file);
int read_node (node_d *new_node, offset_t node_offset, table_ram *table, FILE *file);


/*  IO functions.  */

/*  Writes a table with list of keyspaces.*/
int write_table_descriptor (table_ram *table, FILE *file, char *filename);
/*  Reads a table with list of keyspaces.*/
int read_table_descriptor (table_ram *table, FILE *file);