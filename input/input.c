#include "input.h"
#include <string.h>

FILE *user_file () {
    puts ("Enter filename to read from\nor \"stdin\" to read from stdin");
    char filename [50] = {};
    char *errmsg = "";
    FILE *file = NULL;

    while (!file) {
        puts (errmsg);
        errmsg = "Wrong file";
        if (scanf ("%50s", filename) ==  EOF)
            return NULL;
        if (!strcmp(filename, "stdin"))
            return stdin;
        file = fopen (filename, "r");
    }
    
    return file;
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
        status = get_int_file (&option_num, file);

        if (status == ERREOF)
            return 0;
    
    } while (status == ERRWRG);

    return option_num;
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

