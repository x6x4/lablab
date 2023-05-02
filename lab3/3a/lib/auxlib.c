#include "../lib/auxlib.h"
#include "lib_table_ram.h"
#include <locale.h>
#include "curses.h"

int gui_input (table_t *table) {
        
    setup_ncurses ();
    initial_msg ();
    
    while (1) {
            
            refresh ();

            switch (choice_loop ()) {
            case QUIT:      endwin ();
                            free_table (table);
                            return 0;

            case INSERT:    endwin ();
                            printf ("\nEnter key: ");
                            char key [50] = "";
                            scanf ("%50s", key);

                            printf ("Enter value: ");
                            int val = 0;
                            scanf ("%d", &val);
                            insert_table (table, key, val);

                            break;

            case DELETE:    printw ("Insert [ ] Delete [*] Search [ ] Print [ ]\r");
                            break;
            case FIND:	    printw ("Insert [ ] Delete [ ] Search [*] Print [ ]\r");
                            break;
            case PRINT:	    endwin ();
                            printf ("\n");
                            print_table (table);
                            scanf ("%2[\n]");

                            break;  
            }
        }
}

int choice_loop () {

    static int height = 0;

    size_t c = 0;

    while (c != '\n') {

        /* Wait for user input (one keypress). */
        c = getch ();

        if (c == 'Q' || c == 'q')
            return QUIT;

		switch (c) {
        case KEY_RIGHT: height++;
                        break;
        case KEY_LEFT:  height--;
                        break;
        }

        height = (height % 4 + 4) % 4;

		switch (height) {
        case INSERT:    printw ("Insert [*] Delete [ ] Search [ ] Print [ ]\r");
                        break;
		case DELETE:   	printw ("Insert [ ] Delete [*] Search [ ] Print [ ]\r");
                        break;
		case FIND:	    printw ("Insert [ ] Delete [ ] Search [*] Print [ ]\r");
                        break;
        case PRINT:	    printw ("Insert [ ] Delete [ ] Search [ ] Print [*]\r");
                        break;                        
	    }

        refresh ();
    }

    return height;
}

void initial_msg () {

    /* We "print" stuff to the screen,
     * moving the cursor to the next line '\n',
     * and to the beginning of that line '\r'. */
    printw ("Welcome to table ui. Press Q to quit, Enter to select.\n\r");
	printw ("Insert [*] Delete [ ] Search [ ] Print [ ]\r");

    /* We do need to tell the curses library
     * to update the changes made thus far,
     * i.e. that all changes are visible on screen. */
    refresh ();
}

void setup_ncurses () {

    /* Initialize locale, for wide-character support. */
    setlocale (LC_ALL, "");

    /* Start curses mode. */
    initscr ();

    /* No line buffering. */
    raw ();

    /* No echoing keypresses to the screen. */
    noecho ();

    /* We want F1 et cetera. */
    keypad (stdscr, TRUE);
}