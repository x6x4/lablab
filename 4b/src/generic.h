#pragma once

#include <stdio.h>
#include <assert.h>

#define free_nullify(ptr) { free (ptr); ptr = NULL; }

enum {
	ERREOF = -1,
	ERRSUC,
	ERRWRG,
	ERRUNK, 
	ERRDUP,
	ERRCONT
};

#define RED(s) "\x1b[31m"s"\x1b[0m"
#define YELLOW(s) "\x1b[33m"s"\x1b[0m"

/*  Get string of arbitrary length.  */
char *get_str (FILE *file);
/*  Open file with entered filename for reading.  */
FILE *user_file ();
/*  Ask if user want to do something  */
int user_choice (const char *s);
/*  Return entered option number.  */
int option_choice (const char *msgs[], size_t msgc, FILE *file);
/*  Return entered integer in specified bounds.  */
int get_int_file (FILE *file, int *numptr, int high, int low);