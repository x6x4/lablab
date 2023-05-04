#include "lib_table.h"

/*  Base functions.  */

/*  Inits a table with list of keyspaces.*/
table_t *init_table (size_t klist_max_sz);

/*  Inserts a key-value entry into the table.  */
int insert_table (table_t *table, char *key, int val);

/*  Prints a table.  */
void print_table (table_t *table);


/*  Search functions.  */

/*  Returns number of keyspace with given key.  */
ks_t *ks_by_key (table_t *table, char *key);
/*  Returns nodes with given key and version, NULL if key not found.  */
table_t *node_by_key_ver (table_t *table, char *key, size_t ver);
/*  Returns table of all nodes with given key, NULL if key not found.  */
table_t *nodes_by_key (table_t *table, char *key);


/*  Constructors.  */

/*  Allocates keyspace with given key.  */
ks_t *new_keyspace (table_t *table, char *key);
/*  Adds a new node to keyspace.  */
void new_node (table_t *table, ks_t *ks, int val);
/*  Allocates info with given key and val.  */
info_t *new_info (char *key, int val);


/*  Destructors.  */

/*  Flushes a table.  */
void free_table (table_t *table);

/*  Erases a key-value entry from the table.  */
int erase_from_table_by_key_ver (table_t *table, char *key, size_t ver);

/*  Erases a keyspace with given key.  */
int erase_from_table_by_key (table_t *table, char *key);

/*  Erases a keyspace entry from the table.  */
int erase_ks_from_table (table_t *table, ks_t *ks);

