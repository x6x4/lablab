#include "lib_stack_vec.h"
#include <stdio.h>
#include <assert.h>


enum {
    ERRSUC,
    ERRPOP
};


/*  Vector implementation.  */
 
stack_t *new_stack () {
    stack_t *stack = calloc (1, sizeof *stack);
    stack->capacity = STACK_SZ;
    stack->sz = 0;
    return stack;
}

void push_tok (token_t token, stack_t *stack) {

    if (!is_full (stack)) {
        stack->tokens[stack->sz++] = token;
    }
}

int pop_tok (token_t *token, stack_t *stack) {

    if (is_empty (stack)) {
        return ERRPOP;
    }

    *token = stack->tokens[--stack->sz];

    return ERRSUC;
}

void print_stack (stack_t *stack) {

    if (stack->sz == 0) {
        printf ("' '\n");
        return;
    }
   
    for (size_t i = 0; i < stack->sz; i++) {
        
        if (stack->tokens[i].type == NUM) {
            printf ("%d ", stack->tokens[i].val.num);
        }

        if (stack->tokens[i].type == OPER) {
            printf ("%c ", stack->tokens[i].val.ch);
        }

    }

    printf ("\n");
}

void free_stack (stack_t *stack) {

    if (stack == NULL) {
        printf ("Error: NULL pointer\n");
        return;
    }

    free (stack);
}

int is_full (stack_t *stack) {

    if (stack->sz == stack->capacity) {
        printf ("Error: stack is full\n");
        return 1;
    }    

    return 0;
}

int is_empty (stack_t *stack) {

    if (stack->sz == 0) {
        return 1;
    }    

    return 0;
}