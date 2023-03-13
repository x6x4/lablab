#include <stdlib.h>

/*  
**  Common library.  
*/ 
 
#define ERRVAL __INT_MAX__

typedef struct stack stack_t;

/*  General functions.  */

/*  create empty stack  */
stack_t *new_stack ();

/*  push element to stack */
void push_char (char data, stack_t *stack);
void push_int (int data, stack_t *stack);

/*  pop element from stack */
int pop_char (char *data, stack_t *stack);
int pop_int (int *data, stack_t *stack);

/*  print stack from top to bottom  */
void print_stack (stack_t *stack);

/*  destroy stack  */
void free_stack (stack_t *stack);


/*  State functions.  */

/*  Checks if stack is full.  */
int is_full (stack_t *stack);

/*  Checks if stack is empty.  */
int is_empty (stack_t *stack);
