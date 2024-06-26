#pragma once

#include <stdio.h>
#include <assert.h>
#include <limits.h>

#define free_nullify(ptr) { free (ptr); ptr = NULL; }

#define INF __SIZE_MAX__

typedef short Bool;

enum {
	ERREOF = -1,
	ERRSUC,
	ERRWRG,
	ERRUNK, 
	ERRDUP,
	ERRCONT,
	ERROVF,
	ERRUNF
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
int option_choice (const char *msgs, size_t msgc, FILE *file);
/*  Return entered integer in specified bounds.  */
int get_int_file (FILE *file, int *numptr, int high, int low);
/*  Return entered unsigned in specified bounds.  */
int get_sizet_file (FILE *file, size_t *numptr, size_t high, size_t low);