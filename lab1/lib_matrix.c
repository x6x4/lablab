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

matrix_t *fill_matrix (matrix_t *m, FILE *file) {
 
    assert (file);

    int m_size = 0;
    printf ("Enter number of rows in matrix:\n");
    
    /*  Prepare for new matrix creation.  */
    
    int chrs_rd = fscanf (file, "%d", &m_size);

    /*  Test error cases.  */
    
    if (chrs_rd == 0) {
        while ( fgetc (file) != '\n' );
        printf ("Error: wrong number of rows\n");
        return NULL;
    }
    if (chrs_rd == EOF) {
        printf ("EOF reached\n");
        exit (0);
    }
    if (m_size < 0) {
        printf ("Error: wrong number of rows\n");
        return NULL;
    }

    /*  Create new matrix of size m_size.  */

    m = new_matrix (m_size);
    
    /*  Fill new matrix.  */

    for (int i = 0; i < m->lns_cnt; i++) {
        m->lns[i] = fill_line (m->lns[i], file);
        
        if (m->lns[i] == NULL) {
            free_matrix (m);
            return NULL;
        }
    }

    return m;
}

line_t *fill_line (line_t *l, FILE* file) {
    
    assert (file);

    /*  Prepare for new line creation.  */
    
    int l_size = 0;
    printf ("Enter number of elements in row:\n");

    int chrs_rd = fscanf (file, "%d", &l_size);

    /*  Test error cases.  */
    
    if (chrs_rd == 0) {
        while ( fgetc (file) != '\n' );
        printf ("Error: wrong number of elements\n");
        return NULL;
    }
    if (chrs_rd == EOF) {
        printf ("EOF reached\n");
        exit (0);
    }
    if (l_size < 0) {
        printf ("Error: wrong number of elements\n");
        return NULL;
    }

    /*  Create new line of size l_size.  */
    
    l = new_line (l_size);

    /*  Fill new line.  */

    printf ("Enter elements of row:\n");
    for (int i = 0; i < l->num_cnt; i++) {
        int chrs_rd = fscanf (file, "%d", &l->nums[i]);

        /*  Testing error cases.  */
    
        if (chrs_rd == 0) {
            printf ("Error: wrong element\n");
            while ( fgetc (file) != '\n' );
            free_line (l);
            return NULL;
        }
        if (chrs_rd == EOF) {
            printf ("EOF reached\n");
            free_line (l);
            exit (0);
        }
    }
    
    return l;
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
            if (m->lns[i])
                free_line (m->lns[i]);
        }

        free (m->lns);
        free (m);
    }
}

void free_line (line_t *l) {
    free (l->nums);
    free (l);
}