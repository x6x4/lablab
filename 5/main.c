#include "src/lib_graph.h"
#include <stdlib.h>

int main () {
    struct graph G = {{NULL}, 0};
    print_graph (&G);
    return 0;
}