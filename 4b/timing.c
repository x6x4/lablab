#include "timing.h"
#include <stdio.h>
#include <string.h>
#include <time.h>

int main () {
    timing_tree ();
}

#define ASCII ((rand())%26 + 97)

int timing_tree ()
{
    BNodePtr test_root = NULL;
    
    char test_keys[BUNCH_SZ][2] = {};
    
    unsigned long seed = time(NULL);
    srand (seed);
    //printf ("seed: %u", seed);
    
    for (int epochs_num = 0; epochs_num < EPOCHS_NUM; epochs_num++) {
        for (int bunch_num = 1; bunch_num <= EPOCHS_NUM; bunch_num++) {

            int num_of_keys = BUNCH_SZ*bunch_num;

            /*  Fill test array.  */
            for (int i = 0; i < BUNCH_SZ; ++i) {
                int rand_int = ASCII;
                test_keys[i][0] = (char) rand_int;
                test_keys[i][1] = '\0';
            }

            /*  Fill test root  */
            for (int i = 0; i < num_of_keys; i++) {
                int rand_int = ASCII;
                char rand_key [2] = {(char) rand_int};
                insert_bt (&test_root, rand_key, rand_key);
            }

            timing_search (test_root, test_keys, bunch_num);
            timing_insertion (&test_root, test_keys, bunch_num);
            timing_deletion (&test_root, test_keys, bunch_num);
            //timing_traversal (test_root, test_keys, bunch_num); 

            free_bt (&test_root);   
        }
    }

    return ERREOF;
}

void timing_search (BNodePtr test_root, char test_keys[BUNCH_SZ][2], int bunch_num) {
    clock_t first = 0, last = 0;
    size_t pos = 0;

    first = clock();
    for (int i = 0; i < BUNCH_SZ; ++i) 
        find_bt (test_root, test_keys[i], &pos);

    last = clock();

    printf("%d nodes_num: %d time: %ld\n", bunch_num, bunch_num*BUNCH_SZ, last - first);
}

void timing_insertion (BNodePtr *test_root, char test_keys[BUNCH_SZ][2], int bunch_num) {
    clock_t first = 0, last = 0;

    first = clock();
    for (int i = 0; i < BUNCH_SZ; ++i) 
        insert_bt (test_root, test_keys[i], test_keys[i]);

    last = clock();

    printf("%d nodes_num: %d time: %ld\n", bunch_num, bunch_num*BUNCH_SZ, last - first);
}

void timing_deletion (BNodePtr *test_root, char test_keys[BUNCH_SZ][2], int bunch_num) {
    clock_t first = 0, last = 0;

    first = clock();
    for (int i = 0; i < BUNCH_SZ; ++i) 
        delete_bt (test_root, test_keys[i], 0);

    last = clock();

    printf("%d nodes_num: %d time: %ld\n", bunch_num, bunch_num*BUNCH_SZ, last - first);
}

void timing_traversal (BNodePtr test_root, int bunch_num) {
    clock_t first = 0, last = 0;

    first = clock();
    for (int i = 0; i < BUNCH_SZ; ++i) 
        silent_traverse_bt (test_root);

    last = clock();

    printf("%d nodes_num: %d time: %ld\n", bunch_num, bunch_num*BUNCH_SZ, last - first);
}

void silent_traverse_bt (BNodePtr root) {
    if (!root) 
        return;

    for (size_t i = 0; i < root->csize; i++) 
        silent_traverse_bt (root->child[i]);
        
    silent_traverse_bt (root->child[root->csize]);
}