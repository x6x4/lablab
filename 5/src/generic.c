#include "generic.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *get_str (FILE *file) {

    char *str_val = NULL;
    char *str = NULL;
	size_t len = 0;

    if (getline (&str_val, &len, file) != EOF) {
        str = strdup (str_val);

		/*  getline appends extra \n to to the end of the line  */
	    str[strcspn(str, "\n")] = '\0';
    }
        
    /*  clean buffer allocated by getline  */
    free_nullify (str_val);

    return str;
}

FILE *user_file () {
    char *errmsg = "";
    FILE *file = NULL;

	puts ("Enter filename of file to read from. Enter \"/proc/self/fd/0\" to read from stdin.");

    while (!file) {
        char *filename = get_str (stdin);
        
        if (!filename) 
            return NULL;

        file = fopen (filename, "r");

        free_nullify (filename);
		
		if (!file)
			perror (errmsg);
    }
    
	return file;
}

int user_choice (const char *s) {
    printf ("%s", s);
    int ch = -1;
    
    ch = getchar ();
    //  eat trailing newline
    getchar();
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

int option_choice (const char *msgs, size_t msgc, FILE *file) {
    int option_num = -1;

    puts (msgs);
    
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

int get_sizet_file (FILE *file, size_t *numptr, size_t high, size_t low) {
    const char *errmsg = "";
    int status;

    do {
        printf ("%s", errmsg);
        errmsg = "Bad unsigned\n";
        status = fscanf (file, "%lu", numptr);
        if (status == EOF) {
            return ERREOF;
        }
        while ( fgetc (file) != '\n');
    } while (!status  || (*numptr > high || *numptr < low));

    return ERRSUC;
}