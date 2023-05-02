

#include "src/lib_table_file.h"
#include <stdio.h>
#include <string.h>




int main () {
    char filename [50] = "";
    //printf ("Enter file to work with: ");
    //scanf ("%s", filename);
    strncpy (filename, "table.bin", sizeof "table.bin");
    FILE *file = fopen (filename, "wb");
    
    //  We init table with given parameters or read it from file.
    table_t *table = init_table (64, 16);

    write_table_descriptor (table, file);

    //read_table_descriptor (&table, file);



    fclose (file);


}

/*

#define table->kslist[i] table+size(table)+ks*i


table and ks decriptors are copied to main mem

0x00
table
id [4]
size_t max_sz [4] 64
size_t sz [4] 3
size_t kslist_max_sz [4] 16
size_t kslist_sz [4] 2
int kslist [4]

total 24

0x20
kslist
16 * 4
total 64

0x60
ks
size_t num [4] 1
size_t ks_sz [4] 2
char key [52]
tail [4]

total 64
0xA0

...
16 * 64 = 1024 = 0x400
0x460
node
size_t ver [4]
info [4]
next [4]

total 12

12 * 64 = 0x300
0x760
info 
size_t val [4]
char key [52] 
total 56

56 * 64 = 0xe00

0x1560

*/