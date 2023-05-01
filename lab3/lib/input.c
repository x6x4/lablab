#include  "../lib/auxlib.h"
#include "input.h"

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
                insert_table (table, key, val);
                break;

        case 1: printf ("\nEnter key of keyspace to delete: ");
                fscanf (file, "%50s", key);
                if (erase_from_table_by_key (table, key) == ERRWRG)
                    printf ("Key not found.\n");
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
                break;

        case 3: printf ("\nEnter key of keyspace to search: ");
                fscanf (file, "%50s", key);
                if (!nodes_by_key (table, key)) 
                    printf ("Key not found.\n");
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
                if (!node_by_key_ver (table, key, ver)) 
                    printf ("Item not found.\n");
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

                if (input_file (file, table) == -1) 
                    return ERREOF;
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

