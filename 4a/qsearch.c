#include "src/generic.h"
#include "src/lib_bintree.h"
#include <stddef.h>
#include <stdio.h>


int main () {

    BstNodePtr root = NULL;
    FILE *data = user_binfile ();

    size_t key = 0;

    //  data is a HEX file 
    while (fread (&key, sizeof (size_t), 1, data)) {

        BstNodePtr node = NULL;
        size_t val = 1;

        if (find_by_key_or_ret_par (&node, root, key) == ERRSUC)
            val = node->info->val + 1;

        insert_bst (&root, key, val);
    }

    fclose (data);

    if (!root) {
        printf ("Error: file is too little\n");
        return 1;
    }

    FILE *end_file = freopen ("nums_frequency", "w", stdout);
    traverse_bst (root, NO_KEY);

    fclose (end_file);
    free_bst (&root);
    return 0;
}    
