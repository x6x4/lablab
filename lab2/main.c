#ifdef VEC
#include "src/lib_stack_vec.h"
#else
#include "src/lib_stack_list.h"
#endif

#include <stdio.h>


/*  Various error codes.  */

enum {
    ERREOF = -1, 
    ERRSUC, 
    ERRWRG,
    ERRDIVZ
};


/*  Functions.  */

/*  Takes user input and transforms it to a stack of tokens.  */
int parse (stack_t *stack);

/*  Calculates the value of ariphmetic expression 
given in form of the stack of tokens and puts it in `result_num`.  */
int eval (stack_t *stack, int *result_num);

/*  Takes operator and two operands and puts 
result of the binary operation in `result`.  */
int eval_atomic (const char op, const int elm1, const int elm2, \
                 int *result);

/*  Reads integer and returns error code.  */
int get_int (int *num);



int main () {
    
    int result_num = 0;

    stack_t *stack = new_stack ();

    while (1) {

        switch (parse (stack)) {

            case ERRWRG:
                continue;

            case ERREOF:
                goto end;
            
            default:
                break;
        }

        switch (eval (stack, &result_num)) {

            case ERRWRG:
                continue;
            
            default:
                break;
        }

        printf ("Result: %d\n", result_num);
    
    }

    end:
        free_stack (stack);
        printf ("\nexit\n");
        return 0;
}



int parse (stack_t *stack) {

    int num = 0; char ch = 0; token_t token = {};

    while (1) {

        switch (scanf ("%1[+-/*]", &ch)) {
            
            case 1:
                token.type = OPER;
                token.val.ch = ch;
                push_tok (token, stack);
                break;

            case 0:
                scanf ("%c", &ch);
                if (ch == '\n') {
                    if (stack->sz < 3) {
                        printf ("error: wrong input\n");
                        return 1;
                    }

                    return 0;
                }
                if (ch == ' ') {
                    break;
                }
                ungetc (ch, stdin);

                switch (get_int (&num)) {
                    case ERREOF: 
                        return EOF;
                    case ERRWRG:
                        printf ("error: wrong symbol\n");
                        return 1;
                    default:
                        break;
                }
                
                token.type = NUM;
                token.val.num = num;
                push_tok (token, stack);
                break;

            case -1:
                return EOF;    
        }
            
    }

}

int eval (stack_t *stack, int *result_num) {

    stack_t *stack_num = new_stack ();

    token_t token = {OPER, '+'}; 
    token_t num1 = {}, num2 = {};
    token_t result = {NUM, 0};

    while (!pop_tok (&token, stack)) {

        /*  If token is operand.  */
        if (token.type == NUM) {
            push_tok (token, stack_num);
        } 

        /*  Otherwise it is operator.  */
        else {
        
            /*  Fetch operands.  */
            pop_tok (&num1, stack_num);
            pop_tok (&num2, stack_num);

            /*  Do atomic evaluation.  */
            switch (eval_atomic (token.val.ch, num1.val.num, num2.val.num, \
                &result.val.num)) {
                
                case ERRDIVZ:
                case ERRWRG:
                    return 1;
            
                default:
                    break;
            }

            /*  Push result back to stack.  */
            push_tok (result, stack_num);
            
        }
    }

    free_stack (stack_num);

    *result_num = result.val.num;
    
    return 0;
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
            printf ("error: division by zero!\n");
            return ERRDIVZ;
        }
    }

    else {
        printf ("error: wrong op!\n");
        return ERRWRG;
    }

    return ERRSUC;
}

int get_int (int *num) {

    int chrs_rd = scanf ("%d", num);

    if (chrs_rd == 0) {

        /*  Scan symbols until newline.  */
        scanf ("%*[^\n]%*c");
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