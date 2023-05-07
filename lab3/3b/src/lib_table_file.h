#include "lib_table.h"
#include <stdio.h>

/*  Base functions.  */

int insert_table (table_ram *table, char *key, int val, FILE *file);
void print_table (table_ram *table, FILE *file);
int erase_from_table_by_key_ver (table_ram *table, char *key, size_t ver, FILE *file);
int erase_from_table_by_key (table_ram *table, char *key);
void free_table (table_ram *table);

/*  
    How to return a table, if table is a file in its essence?
    I decided to print all appropriate nodes.  
*/
void print_by_key (table_ram *table, char *key, FILE *file);
void print_by_key_ver (table_ram *table, char *key, size_t ver, FILE *file);

ks_t *ks_by_key (table_ram *table, char *key);

/*  Great and terrible, appears with each 10th item.  */
void garbage_collector (table_ram *table, FILE *file);


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