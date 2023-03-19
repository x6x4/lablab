#ifdef VEC
#include "src/lib_stack_vec.h"
#else
#include "src/lib_stack_list.h"
#endif

#include <stdio.h>

#define INIT_CHAR 0


int eval (stack_t *stack, int *errcode);

int eval_atomic (const char op, const int elm1, const int elm2, int *result);


int main () {
    
    stack_t stack = {NULL, NULL, 0};
    int errcode = 0;

    while (1) {
        printf ("result: %d\n", eval (&stack, &errcode));
        
        if (errcode == EOF) {
            printf ("\nexit\n");
            break;
        }
    }

}


int eval (stack_t *stack, int *errcode) {

    while (1) {
        //  + - 4 5 6
        //  * 4 -5

        int cur_num = 0, res = 0;
        char ch = INIT_CHAR;

        switch (scanf (" %1[+-/*]%d", &ch, &cur_num)) {
        
        case 2:

            eval_atomic (ch, cur_num, eval (stack, errcode), &res);
            
            if (pop_char (&ch, stack) == 0) {
                eval_atomic (ch, res, eval (stack, errcode), &res);
            }

            printf ("res: %d\n", res);
            return res;
        
        case 1:

            if (ch != INIT_CHAR) {
                push_char (ch, stack);
                continue;
            }

            else {
                res = cur_num;
                return res;
            }

        case 0:
            scanf ("%d", &cur_num);
            return cur_num;

        default:
            *errcode = EOF;
            return res;
        }
            
    }
}


int eval_atomic (const char op, const int elm1, const int elm2, int *result) {

    if (0 == (op - '+')) {
        *result = elm1 + elm2;
    }
    else if (0 == (op - '-')) {
        *result = elm1 - elm2;
    }
    else if (0 == (op - '*')) {
        *result = elm1 * elm2;
    }
    else if (0 == (op - '/')) {
        
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