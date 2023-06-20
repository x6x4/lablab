#include <stdio.h>

enum {
	ERREOF = -1,
	ERRSUC,
	ERRWRG,
	ERRUNK, 
	ERRDUP
};

#define RED(s) "\x1b[31m"s"\x1b[0m"
#define CYAN(s) "\x1b[33m"s"\x1b[0m"

/*  Opens file with entered filename for reading.  */
FILE *user_file ();
/*  Asks if user want to do something  */
int user_choice (const char *s);
/*  Returns entered option number.  */
int option_choice (const char *msgs[], size_t msgc, FILE *file);
/*  Returns entered integer in specified bounds.  */
int get_int_file (FILE *file, int *numptr, int high, int low);