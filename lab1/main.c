#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lib_matrix.h"

int main () {
    char input [100] = "";
    char *input_ptr = input;
    char *input_ptr2 = input_ptr;

    /*  Read user input to input.  */
    while (1) {
        input_ptr = input_ptr2;

        if (fgets (input, sizeof input, stdin) == NULL) {
            printf ("\nEOF reached\n");
            return 0;
        }

        input [strlen (input) - 1] = '\0';
        
        matrix_t *m = NULL; 
        m = fill_matrix (m, &input_ptr);

        if (m == NULL) {
            continue;
        }  

        print_matrix (m); 

        /*  Create vector.  */ 
        float *c = calloc (m->lns_cnt, sizeof *c);
        float max = -__INT_MAX__;
        float min = __INT_MAX__;
        for (int i = 0; i < m->lns_cnt; i++) {
            line_t *cur_ln = m->lns[i];

            for (int j = 0; j < cur_ln->num_cnt; j++) {
                c[i] += cur_ln->nums[j];
            }

            if (c[i] > max) {
                max = c[i];
            }
            if (c[i] < min) {
                min = c[i];
            }

        }

        printf ("Vector:\n");
        for (int i = 0; i < m->lns_cnt; i++) {
            if ( (max - min) == 0) {
                printf ("Error creating vector: division by zero\n");
                break;
            }

            c[i] = (float) (c[i] - min) / (max - min);
            printf ("%f ", c[i]);
        }
        printf ("\n\n");

        free_matrix (m);
        free (c);        
    }
}