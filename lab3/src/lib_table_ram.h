#include "lib_table.h"

table_t *init_table (size_t max_sz);
int insert_table (table_t *table, char *key, int val);
void print_table (table_t *table);
void free_table (table_t *table);

node_t *new_node (table_t *table, char *key, int val);
info_t *new_info (char *key, int val);
node_t *node_by_key (table_t *table, char *key);

