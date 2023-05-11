#include "src/lib_table_file.h"
#include "../../input/input.h"
#include <stdio.h>
#include <string.h>
#include <limits.h>

int check_table (FILE *table_disk);

int insert (table_ram *table, FILE *file);
int delete_by_key (table_ram *table, FILE *file);
int delete_by_key_ver (table_ram *table, FILE *file);
int find_by_key (table_ram *table, FILE *file);
int find_by_key_ver (table_ram *table, FILE *file);
int print (table_ram *table, FILE *file);

const char *msgs [] = {"\n0 - quit", "1 - insert", "2 - delete by key",
                    "3 - delete by key + version", "4 - find by key",
                    "5 - find by key + ver", "6 - print"};

const size_t msgc = sizeof msgs / sizeof msgs[0];

int (*fptr[]) (table_ram*, FILE *file) = 
    {NULL, insert, delete_by_key, delete_by_key_ver, 
        find_by_key, find_by_key_ver, print};


static FILE *table_disk;
static char *default_filename = "table.hex";

int main () {

    puts ("Table ui. ^D to quit.");
    table_ram *table = init_table_file (KSLIST_MAX_SZ);

    char filename [14] = "";
    puts ("Enter filename of table file (max 14 symbols) \
    \nor type \"n\" to create new file named \"table.hex\": ");
    scanf ("%14s", filename);
    if (!strcmp (filename, "n")) {
        table_disk = fopen (default_filename, "wb+");
        write_table_descriptor (table, table_disk, default_filename);
    } 
    else {
        table_disk = fopen (filename, "rb+");
        if (check_table (table_disk)) {
            free_table_file (table);
            return 1;
        }
        
        read_table_descriptor (table, table_disk);
    }

    FILE *file = user_file ();
    if (!file) {
        free_table_file (table);
        fclose (table_disk);
        puts ("quit");
        return 0;
    }
    
    size_t fn_num = 0;

    while (fn_num = option_choice (msgs, msgc, file)) {
        //  each function returns 0 if EOF
        if (!fptr[fn_num](table, file))
            break;
    }

    fclose (file);
    write_table_descriptor (table, table_disk, default_filename);
    free_table_file (table);
    fclose (table_disk);
    puts ("quit");
    return 0;
}

int check_table (FILE *table_disk) {
    if (!table_disk) {
        puts ("Error! File not found.");
        return ERRWRG;
    }

    size_t format = 0;
    fseek (table_disk, 0, SEEK_SET);
    fread (&format, 2, 1, table_disk);

    if (format != 0xda7a) {
        puts ("Error! Table not found");
        fclose (table_disk);
        return ERRWRG;
    }    

    return ERRSUC;
}

int insert (table_ram *table, FILE *file) {
    /*  eat newline after option  */
    fscanf (file, "%*c");
    char *key = get_input_string ("\nEnter key of item to insert: ", file);

    size_t val = 0;
    printf ("Enter value of item to insert: ");
    char *errmsg = "";
    int status = -1;

    do {
        puts (errmsg);
        errmsg = "Bad input";
        status = get_int_file (&val, file, INT_MAX, 0);

        if (status == ERREOF) {
            free (key);
            return 0;
        }
    
    } while (status == ERRWRG);

    if (insert_table_file (table, key, val, table_disk) == ERRSUC) 
        printf ("Item inserted successfully.\n");

    free (key);
    return 1; 
}

int delete_by_key (table_ram *table, FILE *file) {
    /*  eat newline after option  */
    fscanf (file, "%*c");
    char *key = get_input_string ("\nEnter key of keyspace to delete: ", file);

    if (erase_from_table_by_key_file (table, key, table_disk) == ERRSUC) 
        printf ("Keyspace deleted successfully.");

    free (key);
    return 1; 
}

int delete_by_key_ver (table_ram *table, FILE *file) {
    /*  eat newline after option  */
    fscanf (file, "%*c");
    char *key = get_input_string ("\nEnter key of item to delete: ", file);
    size_t ver = 0;
    printf ("Enter version of item to delete: ");
    char *errmsg = "";
    int status = -1;

    do {
        puts (errmsg);
        errmsg = "Bad input";
        status = get_int_file (&ver, file, INT_MAX, 0);

        if (status == ERREOF) {
            free (key);
            return 0;
        }
    
    } while (status == ERRWRG);

    if (erase_from_table_by_key_ver_file (table, key, ver, table_disk) == ERRSUC)
        puts ("Item deleted successfully");

    free (key);
    return 1; 
}

int find_by_key (table_ram *table, FILE *file) {
    /*  eat newline after option  */
    fscanf (file, "%*c");
    char *key = get_input_string ("\nEnter key of keyspace to find: ", file);

    print_by_key (table, key, table_disk);
    
    free (key);
    return 1; 
}

int find_by_key_ver (table_ram *table, FILE *file) {
    /*  eat newline after option  */
    fscanf (file, "%*c");
    char *key = get_input_string ("\nEnter key of item to find: ", file);

    size_t ver = 0;
    printf ("Enter version of item to find: ");
    char *errmsg = "";
    int status = -1;

    do {
        puts (errmsg);
        errmsg = "Bad input";
        status = get_int_file (&ver, file, INT_MAX, 0);

        if (status == ERREOF) {
            free (key);
            return 0;
        }
    
    } while (status == ERRWRG);

    print_by_key_ver (table, key, ver, table_disk);

    free (key);
    return 1; 
}

int print (table_ram *table, FILE *file) {
    printf ("Table:\n");
    print_table_file (table, table_disk);
    return 1;
}


