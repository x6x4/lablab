#include "lib_stack_list.h"
#include <stdio.h>
#include <assert.h>

enum {
    ERRSUC,
    ERRPOP
};

stack_t *new_stack () {
    stack_t *stack = calloc (1, sizeof *stack);
    stack->sz = 0;
    stack->top = NULL;
    stack->bottom = NULL;
}

void push_char (char data, stack_t *stack) {
    
    if (stack->sz++ == 0) {
        stack->top = new_node ();

        stack->top->next = stack->top;
        stack->top->data->is_char = 1;
        stack->top->data->ch = data;

        stack->bottom = stack->top;
        
        return;
    }

    node *new_top = new_node ();

    new_top->next = stack->top;
    stack->bottom->next = new_top;

    stack->top = new_top;  
    stack->top->data->is_char = 1;
    stack->top->data->ch = data;
}

void push_int (int data, stack_t *stack) {
    
    if (stack->sz++ == 0) {
        stack->top = new_node ();

        stack->top->next = stack->top;
        stack->top->data->is_char = 0;
        stack->top->data->num = data;

        stack->bottom = stack->top;
        
        return;
    }

    node *new_top = new_node ();

    new_top->next = stack->top;
    stack->bottom->next = new_top;

    stack->top = new_top;  
    stack->top->data->is_char = 0;
    stack->top->data->num = data;
}

int pop_char (char *data, stack_t *stack) {
    if (is_empty (stack)) {
        return ERRPOP;
    }

    *data = stack->top->data->ch;

    node *prev_top = stack->top;

    if (stack->sz-- == 1) {
        free (stack->top->data);
        free (stack->top);
        return ERRSUC;
    }

    stack->top = stack->top->next;
    stack->bottom->next = stack->top;

    free (prev_top->data);
    free (prev_top);
    
    return ERRSUC;
}

int pop_int (int *data, stack_t *stack) {
    if (is_empty (stack)) {
        return ERRPOP;
    }

    *data = stack->top->data->num;

    node *prev_top = stack->top;

    if (stack->sz-- == 1) {
        free (stack->top->data);
        free (stack->top);
        return ERRSUC;
    }

    stack->top = stack->top->next;
    stack->bottom->next = stack->top;

    free (prev_top->data);
    free (prev_top);

    return ERRSUC;
}

void print_stack (stack_t *stack) {

    if (stack->sz == 0) {
        printf ("Stack is empty\n");
        return;
    }

    node *cur_node = stack->top;

    do {
        if (0 == cur_node->data->is_char) {
            printf ("%d ", cur_node->data->num);
        }
        if (1 == cur_node->data->is_char) {
            printf ("%c ", cur_node->data->ch);
        }

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
            free (buf->data);
            free (buf);
        };
    }

    free (stack);
}

int is_empty (stack_t *stack) {
    
    if (stack->sz == 0) {
        return 1;
    }    

    return 0;
}

node *new_node () {
    node *node = calloc (1, sizeof *node);
    assert (node);
    node->data = calloc (1, sizeof *node->data);
    node->data->ch = '\0';
    node->data->num = 0;
    return node;
}
