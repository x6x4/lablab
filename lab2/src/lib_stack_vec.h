#include "lib_stack.h"

#define STACK_SZ 100

struct stack {
    token_t tokens [STACK_SZ];
    size_t sz;
    size_t capacity;
};