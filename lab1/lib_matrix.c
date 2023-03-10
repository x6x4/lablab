#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "lib_matrix.h"

matrix_t *new_matrix (int cnt) {

    matrix_t *m = calloc (1, sizeof *m);
    assert (m);

    m->lns = calloc (cnt, sizeof *m->lns);
    assert (m->lns);

    m->lns_cnt = cnt;

    return m;
}

line_t *new_line (int cnt) {

    line_t *l = calloc (1, sizeof *l);
    assert (l);

    l->nums = calloc (cnt, sizeof *l->nums);
    assert (l->nums);

    l->num_cnt = cnt;

    return l;
}


int get_int (int *num, FILE *file) {

    int chrs_rd = fscanf (file, "%d", num);

    if (chrs_rd == 0) {

        /*  Move fp until newline.  */
        while ( fgetc (file) != '\n' );
        
        return ERRWRG;
    }
    if (chrs_rd == EOF) {
        return ERREOF;
    }
    if (*num < 0) {
        return ERRWRG;
    }

    return ERRSUC;
}

int fill_matrix (matrix_t **m, FILE *file) {
 
    assert (file);

    int m_size = 0;
    printf ("\nEnter number of rows in matrix:\n");
    
    /*  Test error cases.  */

    switch (get_int (&m_size, file))
    {
        case ERRWRG:
            printf ("Error: wrong number of rows\n");
            return ERRWRG;
        case ERREOF:
            printf ("EOF reached\n");
            return ERREOF;
    }    

    /*  Create new matrix of size m_size.  */

    *m = new_matrix (m_size);
    
    /*  Fill new matrix.  */

    for (int i = 0; i < (*m)->lns_cnt; i++) {

        switch (fill_line (&(*m)->lns[i], file))
        {
            case ERRWRG:
                free_matrix (*m);
                return ERRWRG;
        
            case ERREOF:
                free_matrix (*m);
                return ERREOF;
        }
        
    }
    
    return ERRSUC;
}

int fill_line (line_t **l, FILE* file) {
    
    assert (file);

    /*  Prepare for new line creation.  */
    
    int l_size = 0;
    printf ("Enter number of elements in row:\n");

    /*  Test error cases.  */

    switch (get_int (&l_size, file))
    {
        case ERRWRG:
            printf ("Error: wrong number of elements\n");
            return ERRWRG;
        case ERREOF:
            printf ("EOF reached\n");
            return ERREOF;
    } 

    /*  Create new line of size l_size.  */
    
    *l = new_line (l_size);

    /*  Fill new line.  */

    printf ("Enter elements of row (%d):\n", (*l)->num_cnt);
    
    for (int i = 0; i < (*l)->num_cnt; i++) {

        /*  Test error cases.  */
    
        switch (get_int (&(*l)->nums[i], file))
        {
            case ERRWRG:
                printf ("Error: wrong number of elements\n");
                return ERRWRG;
            case ERREOF:
                printf ("EOF reached\n");
                return ERREOF;
        } 

    }
    
    return ERRSUC;
}

void print_matrix (const matrix_t *m) {

    if (m == NULL || (m->lns == NULL)) {
        printf ("Error: can't print uninitialised matrix.\n");
        return;
    }

    printf ("\nMatrix:\n");

    for (int i = 0; i < m->lns_cnt; i++) {
        assert (m->lns[i]);
        print_line (m->lns[i]);
    }

    printf ("\n");
}

void print_line (const line_t *l) {
    if (l == NULL || (l->nums == NULL)) {
        printf ("Error: can't print uninitialised line.\n");
        return;
    }

    for (int i = 0; i < l->num_cnt; i++) {
        printf ("%d ", l->nums[i]);
    }
    printf ("\n");
}

void free_matrix (matrix_t *m) {
    if (m) {
        for (size_t i = 0; i < m->lns_cnt; i++) {
            if (m->lns[i]) {
                free_line (m->lns[i]);
            }
        }

        free (m->lns);
        free (m);
    }
}

void free_line (line_t *l) {
    if (l) {
        if (l->nums) {
            free (l->nums);
        }

        free (l);
    }
}