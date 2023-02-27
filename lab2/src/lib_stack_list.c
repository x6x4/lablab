#include "lib_stack_list.h"
#include <stdio.h>
#include <assert.h>

stack_t *new_stack () {
    stack_t *stack = calloc (1, sizeof *stack);
    stack->sz = 0;
    stack->top = NULL;
}

void push (int data, stack_t *stack) {
    
    if (stack->sz++ == 0) {
        stack->top = new_node ();

        stack->top->next = stack->top;
        stack->top->data = data;
        return;
    }

    node *stack_bottom = stack->top->next;
    stack->top->next = new_node ();
    stack->top = stack->top->next;

    stack->top->next = stack_bottom;
    stack->top->data = data;
}

int pop (stack_t *stack) {
    if (is_empty (stack)) {
        return ERRVAL;
    }

    int data = stack->top->data;

    node *new_top = stack->top;

    if (stack->sz-- == 1) {
        free (stack->top);
        return data;
    }

    do {
        new_top = new_top->next;
    } while (new_top->next != stack->top);

    new_top->next = stack->top->next;

    free (stack->top);
    stack->top = new_top;

    return data;
}

void print_stack (stack_t *stack) {

    if (stack->sz == 0) {
        printf ("Stack is empty\n");
        return;
    }

    node *cur_node = stack->top;

    do {
        printf ("%d ", cur_node->data);
        cur_node = cur_node->next;
    } while (cur_node != stack->top);

    printf ("\n");
}

void free_stack (stack_t *stack) {
    node *cur_node = stack->top;
    if (cur_node) {
        while (stack->sz--) {
            node *buf = cur_node;
            cur_node = cur_node->next;
            free (buf);
        };
    }

    free (stack);
}

int is_empty (stack_t *stack) {
    if (stack->sz == 0) {
        printf ("Error: stack is empty\n");
        return 1;
    }    

    return 0;
}

node *new_node () {
    node *node = calloc (1, sizeof *node);
    assert (node);
    return node;
}
