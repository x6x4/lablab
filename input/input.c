#include "input.h"
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

FILE *user_file () {
    puts ("Enter filename to read from\nor \"stdin\" to read from stdin");
    char *errmsg = "";
    FILE *file = NULL;
    char *filename = NULL;

    while (!file) {
        printf ("%s", errmsg);
        errmsg = "Wrong file\n";
        filename = get_input_string ("", stdin);

        if (!filename) 
            return NULL;
        if (!strcmp(filename, "stdin")) {
            errmsg = "";
            free (filename);
            return stdin;
        }
        file = fopen (filename, "r");
        free (filename);
    }

    return file;
}

char *get_input_string (const char *prompt, FILE *file) {

    char *str = NULL;
    size_t input_size = 0;
    printf ("%s", prompt);

    while (1) {
        if (getline (&str, &input_size, file) == EOF) {
            free (str);
            return NULL;
        }
        
        if (isspace(str[0])) 
            continue;
        
        else 
            break;
    }
    str[strcspn(str, "\n")] = '\0';
    str[strcspn(str, " ")] = '\0';

    return str;
}

int option_choice (const char *msgs[], size_t msgc, FILE *file) {
    int option_num = -1;
    char *errmsg = "";

    for (int i = 0; i < msgc; i++) 
        puts (msgs[i]);
    
    int status = -1;

    do {
        puts (errmsg);
        errmsg = "Bad input";
        status = get_int_file (&option_num, file, msgc - 1, 0);

        if (status == ERREOF)
            return 0;
    
    } while (status == ERRWRG);

    return option_num;
}


int get_int_file (int *num, FILE *file, int high, int low) {

    int chrs_rd = fscanf (file, "%d", num);

    if (chrs_rd == 0) {

        /*  Move fp until newline.  */
        while ( fgetc (file) != '\n' );
        
        return ERRWRG;
    }
    if (chrs_rd == EOF) {
        return ERREOF;
    }

    if (*num > high || *num < low)
        return ERRWRG;

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

