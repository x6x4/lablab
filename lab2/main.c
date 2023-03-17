#ifdef VEC
#include "src/lib_stack_vec.h"
#else
#include "src/lib_stack_list.h"
#endif

#include <stdio.h>

int eval (stack_t stack);

int main () {
    //  * / -30 + 2 3 7
    //  * 8 9
    
    stack_t stack = {NULL, NULL, 0};
    //while (1) {
        //stack_t stack = {NULL, NULL, 0};
        eval (stack);
        printf ("%d\n", eval (stack));
    //}

}

int eval_atomic (const char op, const int elm1, const int elm2, int *result);



int eval (stack_t stack) {

    static int is_first = 0;

    while (1) {
        int cur_num = 0, elm1, elm2, res;
        char ch = 0;

        //int ret = scanf ("%[ -]%d", &ch, &cur_num);
        int ret = scanf ("%[ ]%d", &ch, &cur_num);
        printf ("ret: %d\n", ret);
        //printf ("ch:%ca\n", ch);
        //printf ("num: %d\n", cur_num);
        //if (ch == '-') {
            //cur_num = -cur_num;
        //}

        if (ret == 2) {
            if (is_first) {
                pop_char (&ch, &stack);
                print_stack (&stack);
                is_first = 0;
                eval_atomic (ch, cur_num, eval (stack), &res);
                printf ("1st res: %d\n", res);
                goto end;
            }
            else {
                res = cur_num;
                printf ("2nd res: %d\n", res);
                return res;
            }
        }

        if (ret == 0 || ret == 1) {
            ret = scanf ("%1[+-/*]", &ch);
            if (ret == 0) {
                if (ch == '\0') {
                    return res;
                }
                printf ("Wrong symbol\n");
                break;
            }
            push_char (ch, &stack);
            //print_stack (&stack);
            is_first = 1;
            continue;
        }

        end:
            if (pop_char (&ch, &stack) == 0) {
                eval_atomic (ch, res, eval (stack), &res);
            }
            printf ("res: %d\n", res);
        return res;
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
            //return ERRDIVZERO;
        }
    }

    else {
        printf ("Wrong op!");
        //return ERRWRG;
    }

    //return ERRSUC;
    return 0;
}