#include "lib_stack.h"

#define STACK_SZ 3

struct stack {
    int data [STACK_SZ];
    size_t sz;
    size_t capacity;
};