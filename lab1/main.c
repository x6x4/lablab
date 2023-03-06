#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lib_matrix.h"
#include <limits.h>

#define EPS 1e-5

/*  Creates vector from elements of matrix and prints it.  */
void vector (const matrix_t *m);


int main () {
    char input [100] = "";
    char *input_ptr = input;
    char *input_ptr2 = input_ptr;

    printf ("Enter matrix in following format:                                        \
      \n<number of rows> <number of elements in each row> <elements of each row>      \
      \nWarning: separate characters with only one space, otherwise program won't work correctly.\n");                                 

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
        
        vector (m);
        
        free_matrix (m);    
    }
}

void vector (const matrix_t *m) {

    /*  Create vector.  */ 
    float *c = calloc (m->lns_cnt, sizeof *c);

    float max = INT_MIN;
    float min = INT_MAX;

    /*  Fill vector with sums of elements in each row.  */

    for (int i = 0; i < m->lns_cnt; i++) {
        line_t *cur_ln = m->lns[i];

        for (int j = 0; j < cur_ln->num_cnt; j++) {
            c[i] += cur_ln->nums[j];
        }

        if (c[i] - max > EPS) {
            max = c[i];
        }
        if (min - c[i] > EPS) {
            min = c[i];
        }

    }

    /*  Print vector with changed elements.  */

    printf ("Vector:\n");
    for (int i = 0; i < m->lns_cnt; i++) {
        if ((max - min) < EPS) {
            printf ("Error creating vector: division by zero\n");
            break;
        }

        c[i] = (float) (c[i] - min) / (float) (max - min);
        printf ("%f ", c[i]);
    }

    printf ("\n\n");
    free (c);
}