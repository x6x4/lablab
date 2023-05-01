enum {
	ERREOF = -1,
	ERRSUC,
	ERRWRG
};


/*  Ncurses ui.  */

enum {
	QUIT = -1,
	INSERT,
	DELETE,
	FIND,
	PRINT
};

/*  Returns number of option chosed.  */
int choice_loop ();
/*  Setup and init ncurses window.  */
void setup_ncurses ();
/*  Prints greeting.  */
void initial_msg ();
