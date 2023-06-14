#include <stdio.h>

enum {
	ERREOF = -1,
	ERRSUC,
	ERRWRG
};

/*  Opens file with entered filename for reading.  */
FILE *user_file ();
/*  Returns entered option number.  */
int option_choice (const char *msgs[], size_t msgc, FILE *file);
/*  Returns entered integer in specified bounds.  */
int get_int_file (FILE *file, int *numptr, int high, int low);