#ifdef VEC
#include "src/lib_stack_vec.h"
#else
#include "src/lib_stack_list.h"
#endif

#include <stdio.h>
#include <string.h>

int main () {
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
            return 0;
        }

        else {
            printf ("wrong input\n");
        }
    }

    free_stack (stack);
}


