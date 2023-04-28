
#include "src/lib_table_ram.h"

#define MAX_SIZE 3
#define MAX_KS_SIZE 3

int main () {

    table_t *table = init_table (MAX_SIZE, MAX_KS_SIZE);
    insert_table (table, "ksyusha", 20);
    insert_table (table, "kusha", 40);
    insert_table (table, "ksyusha", 50);
    erase_from_table_by_key_ver (table, "kusha", 1);
    //erase_from_table_by_key_ver (table, "ksyusha", 2);
    //erase_from_table_by_key (table, "kusha");
    //print_table (node_by_key_ver (table, "kusha", 9));
    //print_table (nodes_by_key (table, "kusha"));
    print_table (table);
    //free_table (table);
}

