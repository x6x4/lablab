#ifdef VEC
#include "src/lib_stack_vec.h"
#else
#include "src/lib_stack_list.h"
#endif

#include <stdio.h>
#include <string.h>
#include <limits.h>


void stack_interface ();

/*  Reads expression and pushes it to the stack.  */
int parser (FILE *file);



int main () {


    //parser (stdin); 

    stack_interface ();
}

enum retcodes {
    ERREOF = -1,
    ERRWRG,
    ERRSUC
};

int get_int (int *num, FILE *file) {

    int chrs_rd = fscanf (file, "%d", num);

    if (chrs_rd == 0) {

        /*  Move fp until newline.  */
        while ( fgetc (file) != '\n' );
        /*fscanf("%*[^\n]", file);
        fscanf("%*c", file);*/
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


int parser (FILE *file) {
    int num = 0;
    char ch = '\0';

    stack_t *stack = new_stack ();
    
    while (1) {
        switch (scanf ("%c ", &ch)) {
            case ERRWRG:
                printf ("Error: wrong symbol\n");
                return ERRWRG;
            case ERREOF:
                printf ("EOF reached\n");
                print_stack (stack);
                return ERREOF;
            default:
                printf ("%c\n", ch);
        }

        push (ch, stack);
        print_stack (stack);

        /*switch (get_int (&ch, file))
        {
            case ERRWRG:
                printf ("Error: wrong symbol\n");
                return ERRWRG;
            case ERREOF:
                printf ("EOF reached\n");
                print_stack (stack);
                return ERREOF;
        }

        push (ch, stack);*/

    }
}

enum {
    ERRDIVZERO = INT_MAX
};

int eval_atomic (const char op, const int elm1, const int elm2) {

    if (0 == (op - '+')) {
        return elm1 + elm2;
    }
    if (0 == (op - '-')) {
        return elm1 + elm2;
    }
    if (0 == (op - '*')) {
        return elm1 * elm2;
    }
    if (0 == (op - '/')) {
        
        if (0 != elm2) {
            return elm1 / elm2;
        }
        else {
            return ERRDIVZERO;
        }
    }
    
}

void stack_interface () {
    stack_t *stack = new_stack ();

    printf ("Stack. List of available commands:\n    \
    - push                                     \n    \
    - pop                                      \n    \
    - print                                    \n    \
    - exit                                     \n\n");

    while (1)
    {
        char input [10] = "";
        if (fgets (input, sizeof input, stdin) == NULL) {
            break;
        }

        else if ( strstr (input, "push") ) {
            int elm;
            sscanf (input, "push %d", &elm);
            push (elm, stack);
        }

        else if ( strstr (input, "pop") ) {
            int elm = pop (stack);
            if (elm != ERRVAL) {
                printf ("Popped value: %d\n", elm);
            }
        }

        else if ( strstr (input, "print") ) {
            print_stack (stack);
        }

        else if ( strstr (input, "exit") ) {
            free_stack (stack);
            return;
        }

        else {
            printf ("wrong input\n");
        }
    }

    free_stack (stack);
}
