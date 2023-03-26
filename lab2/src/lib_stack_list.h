#include "lib_stack.h"

typedef struct list_node node;

struct list_node {
    node *next;
    token_t token;
};

struct stack {
    node *top;
    node *bottom;
    size_t sz;
};

node *new_node ();