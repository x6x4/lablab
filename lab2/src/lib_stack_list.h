#include "lib_stack.h"

typedef struct list_node node;

typedef struct data_t data_t;

struct list_node {
    node *next;
    data_t *data;
};

struct data_t {
    int is_char;
    char ch;
    int num;
};
 

struct stack {
    node *top;
    node *bottom;
    size_t sz;
};

node *new_node ();