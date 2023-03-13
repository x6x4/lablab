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
stack_t *parser (FILE *file);
/*  Evaluate stack.  */
int eval (FILE *file);

int eval_atomic (const char op, const int elm1, const int elm2);

int main () {


    eval (stdin);

    //stack_interface ();
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


stack_t *parser (FILE *file) {
    int num = 0;
    char ch = '\0';

    stack_t *stack = new_stack ();
    
    while (EOF != scanf (" %c ", &ch)) {
        push_char (ch, stack);
    }

    return stack;
}

enum {
    ERRDIVZERO = INT_MAX
};


int eval (FILE *file) {
    stack_t *stack = parser (file);
    
    //print_stack (stack);

    int res;

    while (1) {
        int elm2 = pop_char (stack);
        int elm1 = pop_char (stack);
        if ('\0' == elm1) {
            break;
        }
        char op = pop_char (stack);

        res = eval_atomic (op, elm1 - 48, elm2 - 48);
    
        if (ERRDIVZERO == res) {
            printf ("Error: division by zero\n");
            return 1;
        }

        push_char (res + 48, stack);
    }

    printf ("res: %d\n", res);

    return 0;
}



int eval_atomic (const char op, const int elm1, const int elm2) {

    if (0 == (op - '+')) {
        return elm1 + elm2;
    }
    if (0 == (op - '-')) {
        return elm1 - elm2;
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
        char input [15] = "";
        if (fgets (input, sizeof input, stdin) == NULL) {
            break;
        }

        else if ( strstr (input, "pushint") ) {
            int elm;
            sscanf (input, "pushint %d", &elm);
            push_int (elm, stack);
        }

        else if ( strstr (input, "pushchar") ) {
            char elm;
            sscanf (input, "pushchar %c", &elm);
            push_char (elm, stack);
        }

        else if ( strstr (input, "popint") ) {
            int elm = pop_int (stack);
            if (elm != ERRVAL) {
                printf ("Popped value: %d\n", elm);
            }
        }

        else if ( strstr (input, "popchar") ) {
            char elm = pop_char (stack);
            if (elm != '\0') {
                printf ("Popped value: %c\n", elm);
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
