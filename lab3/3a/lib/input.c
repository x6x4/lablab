#include  "../lib/auxlib.h"
#include "input.h"

//  TODO: new dialog

int input_file (FILE *file, table_t *table) {

    int option = 0;
    char key [50] = "";
    int val = 0;
    size_t ver = 0;

    while (1) {

        printf ("\nTable ui.\n"
        "0 - insert\n"
        "1 - delete by key\n"
        "2 - delete by key + version\n"
        "3 - search by key\n"
        "4 - search by key + ver\n"
        "5 - print\n"
        "6 - quit\n"
        "7 - import data from file\n");


        switch (get_int_file (&option, file)) {
            case ERREOF: return ERREOF;
            case ERRSUC: break;
            case ERRWRG: printf ("Bad input\n");
                            continue;
        }

        switch (option) {
        case 0: printf ("\nEnter key of item to insert: ");
                fscanf (file, "%50s", key);
                printf ("Enter value of item to insert: ");
                switch (get_int_file (&val, file)) {
                    case ERREOF: return ERREOF;
                    case ERRSUC: break;
                    case ERRWRG: printf ("Bad input\n");
                                 continue;
                }
                if (insert_table (table, key, val) == ERRSUC) 
                    printf ("Item inserted successfully.");
                break;

        case 1: printf ("\nEnter key of keyspace to delete: ");
                fscanf (file, "%50s", key);
                if (erase_from_table_by_key (table, key) == ERRWRG)
                    printf ("Key not found.\n");
                else 
                    printf ("Keyspace deleted successfully.\n");
                break;

        case 2: printf ("\nEnter key of item to delete: ");
                fscanf (file, "%50s", key);
                printf ("Enter version of item to delete: ");
                switch (get_int_file (&ver, file)) {
                    case ERREOF: return ERREOF;
                    case ERRSUC: break;
                    case ERRWRG: printf ("Bad input\n");
                                 continue;
                }
                if (erase_from_table_by_key_ver (table, key, ver) == ERRWRG)
                    printf ("Item not found.\n");
                else 
                    printf ("Item deleted successfully.\n");
                break;

        case 3: printf ("\nEnter key of keyspace to search: ");
                fscanf (file, "%50s", key);
                table_t *key_table = nodes_by_key (table, key);
                if (!key_table) 
                    printf ("Key not found.\n");
                else {
                    print_table (key_table);
                    free_table (key_table);
                }
                break;

        case 4: printf ("\nEnter key of item to search: ");
                fscanf (file, "%50s", key);
                printf ("Enter version of item to search: ");
                switch (get_int_file (&ver, file)) {
                    case ERREOF: return ERREOF;
                    case ERRSUC: break;
                    case ERRWRG: printf ("Bad input\n");
                                 continue;
                }
                key_table = node_by_key_ver (table, key, ver);
                if (!key_table) 
                    printf ("Item not found.\n");
                else {
                    print_table (key_table);
                    free_table (key_table);
                }
                break;

        case 5: printf ("Table:\n");
                print_table (table);
                break;
        
        case 6: return -1;

        if (file == stdin) {

        case 7: printf ("Enter filename (50 symbols max): ");
                char filename [50] = "";
                scanf ("%50s", filename);
                
                FILE *file = fopen (filename, "r");

                if (!file) {
                    printf ("Error: bad file\n");
                    continue;
                }

                if (input_file (file, table) == -1) {
                    fclose (file);
                    return ERREOF;
                }
        }

        default:    while ( fgetc (file) != '\n' );
                    printf ("Bad input\n");
                    continue;
        }

    }
}



int get_int_file (int *num, FILE *file) {

    int chrs_rd = fscanf (file, "%d", num);

    if (chrs_rd == 0) {

        /*  Move fp until newline.  */
        while ( fgetc (file) != '\n' );
        
        return ERRWRG;
    }
    if (chrs_rd == EOF) {
        return ERREOF;
    }
    if (*num < 0) {
        return ERRWRG;
    }

    return ERRSUC;
}

int get_int (int *num) {

    int chrs_rd = scanf ("%d", num);

    if (chrs_rd == 0) {
        
        scanf ( "%*[^\n]%*c");

        return ERRWRG;
    }
    if (chrs_rd == EOF) {
        return ERREOF;
    }
    if (*num < 0) {
        return ERRWRG;
    }

    return ERRSUC;
}

