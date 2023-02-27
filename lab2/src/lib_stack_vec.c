#include "lib_stack_vec.h"
#include <stdio.h>

/*  Vector implementation.  */

stack_t *new_stack () {
    stack_t *stack = calloc (1, sizeof *stack);
    stack->capacity = STACK_SZ;
    stack->sz = 0;
    return stack;
}

void push (int elm, stack_t *stack) {
    if (!is_full (stack)) {
        stack->data[stack->sz++] = elm;
    }
}

int pop (stack_t *stack) {
    if (is_empty (stack)) {
        return ERRVAL;
    }
        
    int elm = stack->data[--stack->sz];
    return elm;
}

void print_stack (stack_t *stack) {
    if (stack->sz == 0) {
        printf ("Stack is empty\n");
        return;
    }
    
    for (size_t i = 0; i < stack->sz; i++) {
        printf ("%d ", stack->data[i]);
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
        printf ("Error: stack is empty\n");
        return 1;
    }    

    return 0;
}