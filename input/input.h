#include <stdlib.h>
#include <stdio.h>

enum {
	ERREOF = -1,
	ERRSUC,
	ERRWRG
};

/*  prints prompt and returns entered string  */
char *get_input_string (const char *prompt, FILE *file);
/*  asks user for file to read from, returns NULL if EOF  */
FILE *user_file ();
/*  returns number of option chosed or 0 if EOF  */
int option_choice (const char *msgs[], size_t msgc, FILE *file);

/*  returns read integer in num in success, otherwise returns ERRWRG or ERREOF  */
int get_int_file (int *num, FILE *file, int high, int low);
/*  returns read integer in num in success, otherwise returns ERRWRG or ERREOF  */
int get_int (int *num);
