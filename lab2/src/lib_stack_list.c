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

void push_tok (token_t token, stack_t *stack) {
    
    if (stack->sz++ == 0) {
        stack->top = new_node ();

        stack->top->next = stack->top;
        stack->top->token = token;

        stack->bottom = stack->top;
        
        return;
    }

    node *new_top = new_node ();

    new_top->next = stack->top;
    stack->bottom->next = new_top;

    stack->top = new_top;  
    stack->top->token = token;
}

int pop_tok (token_t *token, stack_t *stack) {

    if (is_empty (stack)) {
        return ERRPOP;
    }

    *token = stack->top->token;

    node *prev_top = stack->top;

    if (stack->sz-- == 1) {
        free (stack->top);
        return ERRSUC;
    }

    stack->top = stack->top->next;
    stack->bottom->next = stack->top;

    free (prev_top);
    
    return ERRSUC;
}

void print_stack (stack_t *stack) {

    if (stack->sz == 0) {
        printf ("' '\n");
        return;
    }

    node *cur_node = stack->top;

    do {
        if (cur_node->token.type == NUM) {
            printf ("%d ", cur_node->token.val.num);
        }
        if (cur_node->token.type == OPER) {
            printf ("%c ", cur_node->token.val.ch);
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
            free (buf);
        };
    }

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
    return node;
}
