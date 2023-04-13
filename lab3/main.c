
#include "src/lib_table_ram.h"

#define MAX_SIZE 3

int main () {

    table_t *table = init_table (MAX_SIZE);
    insert_table (table, "ksyusha", 20);
    insert_table (table, "kusha", 40);
    insert_table (table, "ksyusha", 50);
    print_table (table);
    free_table (table);
}

