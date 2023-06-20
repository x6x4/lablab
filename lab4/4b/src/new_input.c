#include "generic.h"
#include <stddef.h>
#include <stdio.h>
#include <string.h>

FILE *user_file () {
    puts ("Enter filename (50 symbols max) to read from");
    char filename [51] = {};
    char *errmsg = "";
    FILE *file = NULL;

    while (!file) {
        printf ("%s", errmsg);
        errmsg = "Wrong file\n";
        if (scanf ("%50s", filename) ==  EOF)
            return NULL;
        file = fopen (filename, "r");
    }
    
    return file;
}

int user_choice (const char *s) {
    printf ("%s", s);
    int ch = -1;
    
    ch = getchar ();
    switch (ch) {
        case 'y':
            return ERRSUC;
        case 'n':
            return ERRWRG;
        case EOF:
            return ERREOF;
        default:
            return ERRUNK;
    }
}

int option_choice (const char *msgs[], size_t msgc, FILE *file) {
    int option_num = -1;
    char *errmsg = "";

    for (int i = 0; i < msgc; i++) 
        puts (msgs[i]);
    
    int status = get_int_file (file, &option_num, msgc - 1, 0);

    if (status == ERREOF)
        return 0;
    
    return option_num;
}


int get_int_file (FILE *file, int *numptr, int high, int low) {
    const char *errmsg = "";
    int status;

    do {
        printf ("%s", errmsg);
        errmsg = "Bad integer\n";
        status = fscanf (file, "%d", numptr);
        if (status == EOF) {
            return ERREOF;
        }
        while ( fgetc (file) != '\n');
    } while (!status  || (*numptr > high || *numptr < low));

    return ERRSUC;
}

