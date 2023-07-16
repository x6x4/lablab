#include "timing.h"
#include <stdio.h>
#include <string.h>
#include <time.h>


int main () {
    timing_tree ();
}


void timing_tree ()
{
    BNodePtr test_root = NULL;

    /*  array of 10-symbol strings  */

    char test_keys[BUNCH_SZ][LEN] = {};

    /*  set random seed  */

    unsigned long seed = time(NULL);
    srand (seed);

    /*  main benchmark loop  */

    for (int epochs_num = 0; epochs_num < EPOCHS_NUM; epochs_num++) {
        for (int bunch_num = 1; bunch_num <= EPOCHS_NUM; bunch_num++) {

            int num_of_keys_in_test_root = BUNCH_SZ*bunch_num;

            /*  Fill test array.  */
            for (int i = 0; i < BUNCH_SZ; ++i) {
                
                /*  Fill string with random letters.  */
                for (int j = 0; j < LEN - 1; ++j) 
                    test_keys[i][j] = (char) LOWCASE_ALPHA;
                test_keys[i][LEN - 1] = '\0';

            }

            /*  Fill test root.  */
            for (int i = 0; i < num_of_keys_in_test_root; i++) {

                char rand_key [LEN] = {};

                /*  Fill string with random letters.  */
                for (int j = 0; j < LEN - 1; ++j) 
                    rand_key[j] = (char) LOWCASE_ALPHA;
                rand_key[LEN - 1] = '\0';

                insert_bt (&test_root, rand_key, rand_key);
            }

            timing_search (test_root, test_keys, bunch_num);
            timing_insertion (&test_root, test_keys, bunch_num);
            timing_deletion (&test_root, test_keys, bunch_num);
            //timing_traversal (test_root, test_keys, bunch_num); 

            free_bt (&test_root);   
        }
    }
}


void timing_search (BNodePtr test_root, char test_keys[BUNCH_SZ][LEN], int bunch_num) {
    size_t pos = 0;

    clock_t first = clock();

    for (int i = 0; i < BUNCH_SZ; ++i) 
        find_bt (test_root, test_keys[i], &pos);

    clock_t last = clock();

    printf("#%d nodes_num: %d time: %ld\n", bunch_num, bunch_num*BUNCH_SZ, last - first);
}


void timing_insertion (BNodePtr *test_root, char test_keys[BUNCH_SZ][LEN], int bunch_num) {

    clock_t first = clock();

    for (int i = 0; i < BUNCH_SZ; ++i) 
        insert_bt (test_root, test_keys[i], test_keys[i]);

    clock_t last = clock();

    printf("#%d nodes_num: %d time: %ld\n", bunch_num, bunch_num*BUNCH_SZ, last - first);
}


void timing_deletion (BNodePtr *test_root, char test_keys[BUNCH_SZ][LEN], int bunch_num) {

    clock_t first = clock();

    for (int i = 0; i < BUNCH_SZ; ++i) 
        delete_bt (test_root, test_keys[i], 0);

    clock_t last = clock();

    printf("#%d nodes_num: %d time: %ld\n", bunch_num, bunch_num*BUNCH_SZ, last - first);
}


void timing_traversal (BNodePtr test_root, int bunch_num) {
    
    clock_t first = clock();

    for (int i = 0; i < BUNCH_SZ; ++i) 
        silent_traverse_bt (test_root);

    clock_t last = clock();

    printf("#%d nodes_num: %d time: %ld\n", bunch_num, bunch_num*BUNCH_SZ, last - first);
}


void silent_traverse_bt (BNodePtr root) {

    if (!root) 
        return;

    for (size_t i = 0; i < root->csize; i++) 
        silent_traverse_bt (root->child[i]);
        
    silent_traverse_bt (root->child[root->csize]);
}