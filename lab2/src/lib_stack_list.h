#include "lib_stack.h"

typedef struct list_node node;

struct list_node
{
    node *next;
    int data;
};


struct stack {
    node *top;
    size_t sz;
};

node *new_node ();