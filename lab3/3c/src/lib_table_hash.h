#include "lib_table.h"

/*  Base functions.  */

/*  Simple hash function.  */
size_t hash_func (char *key, table_t *table);

/*  Inits a table with list of keyspaces.*/
table_t *init_table (size_t klist_max_sz);

/*  Inserts a key-value entry into the table.  */
int insert_table (table_t *table, char *key, int val);
/*  Checks for insert error conditions.  */
int insert_checks (table_t *table, char *key, int val);

/*  Prints a table.  */
void print_table (table_t *table);


/*  Search functions.  */

/*  Returns table of all nodes with given key, NULL if key not found.  */
table_t *nodes_by_key (table_t *table, char *key);


/*  Constructors.  */

/*  Adds a new node to hashspace.  */
void new_ks (table_t *table, char *key, int val);
/*  Allocates info with given key and val.  */
info_t *new_info (char *key, int val);


/*  Destructors.  */

/*  Flushes a table.  */
void free_table (table_t *table);

/*  Erases an item with given key.  */
int erase_from_table_by_key (table_t *table, char *key);

