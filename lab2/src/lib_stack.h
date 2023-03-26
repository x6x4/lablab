#include <stdlib.h>

/*  
**  Common library.  
*/ 
 
typedef enum type type_t;

enum type {
    NUM, 
    OPER
};

typedef union value val_t;

union value {
    int num;
    char ch;
};

typedef struct token token_t;

struct token {
    type_t type;
    val_t val;    
};

typedef struct stack stack_t;


/*  General functions.  */

/*  initialize empty stack statically */
stack_t init_stack ();

/*  create empty stack dynamically  */
stack_t *new_stack ();

/*  push element to stack */
void push_tok (token_t token, stack_t *stack);

/*  pop element from stack */
int pop_tok (token_t *token, stack_t *stack);

/*  print stack from top to bottom  */
void print_stack (stack_t *stack);

/*  destroy stack  */
void free_stack (stack_t *stack);


/*  State functions.  */

/*  Checks if stack is full.  */
int is_full (stack_t *stack);

/*  Checks if stack is empty.  */
int is_empty (stack_t *stack);
