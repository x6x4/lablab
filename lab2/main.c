#ifdef VEC
#include "src/lib_stack_vec.h"
#else
#include "src/lib_stack_list.h"
#endif

#include <stdio.h>
#include <string.h>
#include <limits.h>


/*  Reads expression and pushes it to the stack.  */
stack_t *parser (FILE *file);
/*  Evaluate stack.  */
int eval (FILE *file);
/*  Elementary evaluation.  */
int eval_atomic (const char op, const int elm1, const int elm2, int *result);


int main () {
    eval (stdin);
}


enum retcodes {
    ERREOF = -1,
    ERRSUC,
    ERRWRG,
    ERRDIVZERO
};


int eval (FILE *file) {

    stack_t *stack = parser (file);

    int res;

    while (1) {
        char elm2, elm1;
        pop_char (&elm2, stack);
        if (1 == pop_char (&elm1, stack)) {
            break;
        }

        char op;
        pop_char (&op, stack);

        int retcode = eval_atomic (op, elm1 - 48, elm2 - 48, &res);
    
        if (ERRDIVZERO == retcode) {
            printf ("Error: division by zero\n");
            free_stack (stack);
            return 1;
        }

        push_char (res + 48, stack);
    }

    printf ("res: %d\n", res);

    free_stack (stack);

    return 0;
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

int eval_atomic (const char op, const int elm1, const int elm2, int *result) {

    if (0 == (op - '+')) {
        *result = elm1 + elm2;
    }
    if (0 == (op - '-')) {
        *result = elm1 - elm2;
    }
    if (0 == (op - '*')) {
        *result = elm1 * elm2;
    }
    if (0 == (op - '/')) {
        
        if (0 != elm2) {
            *result = elm1 / elm2;
        }
        else {
            return ERRDIVZERO;
        }
    }

    return ERRSUC;
    
}

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
