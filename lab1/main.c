#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lib_matrix.h"
#include <limits.h>

#define EPS 1e-5

/*  Creates vector from elements of matrix and prints it.  */
void vector (const matrix_t *m);


int main () {

    while (1) {        
        matrix_t *m = NULL; 
        m = fill_matrix (m, stdin);

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
    line_t *c = new_line (m->lns_cnt);
    //float *c = calloc (m->lns_cnt, sizeof *c);

    float max = INT_MIN;
    float min = INT_MAX;

    /*  Fill vector with sums of elements in each row.  */

    for (int i = 0; i < m->lns_cnt; i++) {
        line_t *cur_ln = m->lns[i];

        for (int j = 0; j < cur_ln->num_cnt; j++) {
            //c[i] += cur_ln->nums[j];
            c->nums[i] += cur_ln->nums[j];
        }

        if (c->nums[i] - max > EPS) {
            //max = c[i];
            max = c->nums[i];
        }
        if (min - c->nums[i] > EPS) {
            //min = c[i];
            min = c->nums[i];
        }

    }

    /*  Print vector with changed elements.  */

    int err = 0;

    printf ("Vector:\n");
    
    for (int i = 0; i < m->lns_cnt; i++) {
        if ((max - min) < EPS) {
            printf ("Error creating vector: division by zero");
            err = 1;
            break;
        }

        c->nums[i] = (float) (c->nums[i] - min) / (float) (max - min);
        //printf ("%f ", c[i]);
    }
    
    if (err == 0) {
        print_line (c);
    }

    printf ("\n");
    free_line (c);
}