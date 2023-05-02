#include "lib/input.h"
#include <stdio.h>

#define MAX_SIZE 3
#define MAX_KS_SIZE 2

int main () {

    table_t *table = init_table (MAX_SIZE, MAX_KS_SIZE);

    if (input_file (stdin, table) == -1) {
        free_table (table);
        return 0;
    }

}

