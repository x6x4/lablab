#ifdef VEC
#include "src/lib_stack_vec.h"
#else
#include "src/lib_stack_list.h"
#endif

#include <stdio.h>
#include <string.h>
#include <limits.h>

enum retcodes {
    ERREOF = -1,
    ERRSUC,
    ERRWRG,
    ERRDIVZERO
};

/*  Reads expression and pushes it to the stack.  */
int parser (FILE *file, stack_t *stack);
/*  Evaluate stack.  */
int eval (FILE *file);
/*  Elementary evaluation.  */
int eval_atomic (const char op, const int elm1, const int elm2, int *result);

int reverse_string (char *str);
int get_int (int *num, FILE *file);

int main () {

    char string[100] = {};
    char *str = string;

    while (1) {
        reverse_string (str);
        puts (str);
    }
    

    //while (eval (stdin) != ERREOF);
    return 0;
}

int reverse_string (char *str) {
    
    char ch = {0};
    int counter = 0;

    while (1) {
        
        int retcode = scanf ("%1[ +-/*0-9\n]", &ch); 
        
        if (retcode == EOF) {
            return ERREOF;
        }

        if (retcode == 0) {
            //scanf("%*[^\n]%*c");
            puts ("Wrong symbol");
            return ERRWRG;
        }

        if (ch == '\n') {
            puts ("nl");
            break;        
        }
        
        //if (ch != ' ') {
            str[counter] = ch;
            counter++;
            //puts (str);
        //}
    }

    for (int i = 0; i < counter/2; i++) {
        char buf = str[i];
        str[i] = str[(counter - 1) - i];
        str[(counter - 1) - i] = buf;
    }

    str[counter] = '\0';

    return ERRSUC;

}

int parse_prefix (FILE *file) {

    stack_t *stack = new_stack ();

    int cur_num, result = 0;
    char ch = {0};

    while (1) {
        switch (get_int (&cur_num, file))
        {
        case ERRSUC:
            push_int (cur_num, stack);
            break;
        
        case ERRWRG:
            scanf ("%c", ch);
            int elm1 = 0, elm2 = 0;
            pop_int (&elm1, stack);
            pop_int (&elm1, stack);
            eval_atomic (ch, elm1, elm2, &result);
            push_int (result, stack);

        default:
            print_stack (stack);
            return ERREOF;
        }
    }

   

}
































int eval (FILE *file) {

    stack_t *stack = new_stack ();

    int retcode = parser (file, stack);

    if (retcode == ERREOF) {
        free_stack (stack);
        return ERREOF;
    }

    if (retcode == ERRWRG) {
        free_stack (stack);
        printf ("Wrong input\n");
        return ERRWRG;
    }

    int res;

    while (1) {
        char elm2, elm1;
        pop_char (&elm2, stack);
        if (1 == pop_char (&elm1, stack)) {
            break;
        }

        char op;
        pop_char (&op, stack);

        int retcode = eval_atomic (op, elm1 - '0', elm2 - '0', &res);
    
        if (ERRDIVZERO == retcode) {
            printf ("Error: division by zero\n");
            free_stack (stack);
            return 1;
        }

        push_char (res + '0', stack);
    }

    printf ("res: %d\n", res);

    free_stack (stack);

    return 0;
}

int parser (FILE *file, stack_t *stack) {
    int num = 0;
    char ch = '\0';
    int retcode;

    while (1) {

        retcode = scanf ("%c", &ch); 
        
        if (retcode == EOF) {
            return ERREOF;
        }

        if (retcode == 0) {
            return ERRWRG;
        }

        if (ch == '\n') {
            break;
        }
        if (ch != ' ') {
            push_char (ch, stack);
        }
    }

    print_stack (stack);

    return ERRSUC;
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
