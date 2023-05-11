#include "lib_table.h"
#include <stdio.h>

/*  Base functions.  */

int insert_table_file (table_ram *table, char *key, int val, FILE *file);
void print_table_file (table_ram *table, FILE *file);
int erase_from_table_by_key_ver_file (table_ram *table, char *key, size_t ver, FILE *file);
int erase_from_table_by_key_file (table_ram *table, char *key, FILE *file);
void free_table_file (table_ram *table);

/*  Search and print.  */ 
void print_by_key (table_ram *table, char *key, FILE *file);
void print_by_key_ver (table_ram *table, char *key, size_t ver, FILE *file);
ks_d *ks_by_key_file (table_ram *table, char *key, FILE *file);

/*  Helper functions.  */
void garbage_collector (table_ram *table, FILE *file);
char *read_string (offset_t str_offset, FILE *file);

/*  Constructors.  */
table_ram *init_table_file (size_t kslist_max_sz);
ks_d *new_keyspace_file (table_ram *table, char *key, FILE *file);
void new_node_file (table_ram *table, ks_d *ks, int val, FILE *file);
int read_node (node_d *new_node, offset_t node_offset, table_ram *table, FILE *file);


/*  IO functions.  */

/*  Writes a table with list of keyspaces.*/
int write_table_descriptor (table_ram *table, FILE *file, char *filename);
/*  Reads a table with list of keyspaces.*/
int read_table_descriptor (table_ram *table, FILE *file);