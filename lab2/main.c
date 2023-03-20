#ifdef VEC
#include "src/lib_stack_vec.h"
#else
#include "src/lib_stack_list.h"
#endif

#include <stdio.h>

#define INIT_CHAR 0


int eval ();

int eval_atomic (const char op, const int elm1, const int elm2, int *result);


int main () {

    while (1) {
        
        if (eval () == EOF) {
            printf ("\nexit\n");
            break;
        }
    }

}


int eval () {

    stack_t stack_int = {NULL, NULL, 0};
    stack_t stack_char = {NULL, NULL, 0};

    int num = 0; char ch = 0;

    while (1) {

        switch (scanf ("%1[+-/*]", &ch)) {
            
            case 1:
                push_char (ch, &stack_char);
                break;

            case 0:
                scanf ("%c", &ch);
                if (ch == '\n') {
                    print_stack (&stack_char);
                    print_stack (&stack_int);
                    return 1;
                }
                if (ch == ' ') {
                    break;
                }
                ungetc (ch, stdin);

                scanf ("%d", &num);
                push_int (num, &stack_int);
                break;

            case -1:
                print_stack (&stack_char);
                print_stack (&stack_int);
                return EOF;    
        }
            
    }

}


int eval_atomic (const char op, const int elm1, const int elm2, int *result) {

    if      (op =='+') {
        *result = elm1 + elm2;
    }
    else if (op == '-') {
        *result = elm1 - elm2;
    }
    else if (op == '*') {
        *result = elm1 * elm2;
    }
    else if (op == '/') {
        
        if (0 != elm2) {
            *result = elm1 / elm2;
        }
        else {
            printf ("Division by zero!");
        }
    }

    else {
        printf ("Wrong op!");
    }

    return 0;
}