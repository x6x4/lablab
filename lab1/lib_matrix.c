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

matrix_t *fill_matrix (matrix_t *m, char **input) {
    if (input == NULL || *input == NULL) {
        printf ("Error: wrong input\n");
        return NULL;
    }

    /*  Prepare for new matrix creation.  */
    int m_size = 0;
    int chrs_rd = sscanf (*input, "%d", &m_size);

    /*  Test error cases.  */
    if (chrs_rd == 0) {
        printf ("Error: wrong number of rows\n");
        return NULL;
    }
    if (chrs_rd == EOF) {
        printf ("Error: unexpected EOF\n");
        return NULL;
    }
    if (m_size < 0) {
        printf ("Error: wrong number of rows\n");
        return NULL;
    }

    /*  Add whitespace offset. */
    *input += chrs_rd + 1;

    /*  Create new matrix of size m_size.  */
    m = new_matrix (m_size);
    //printf ("Number of rows: %d\n", m->lns_cnt);
    
    /*  Fill new matrix.  */
    for (int i = 0; i < m->lns_cnt; i++) {
        m->lns[i] = fill_line (m->lns[i], input);
        
        if (m->lns[i] == NULL) {
            free_matrix (m);
            return NULL;
        }
    }

    return m;
}

line_t *fill_line (line_t *l, char **input) {
    assert (input);
    assert (*input);

    /*  Prepare for new line creation.  */
    int l_size = 0;
    int chrs_rd = sscanf (*input, "%d", &l_size);

    /*  Test error cases.  */
    if (chrs_rd == 0) {
        printf ("Error: wrong number of elements\n");
        return NULL;
    }
    if (chrs_rd == EOF) {
        printf ("Error: unexpected EOF\n");
        return NULL;
    }
    if (l_size < 0) {
        printf ("Error: wrong number of elements\n");
        return NULL;
    }

    /*  Add whitespace offset. */
    *input += chrs_rd + 1;

    /*  Create new line of size l_size.  */
    l = new_line (l_size);
    //printf ("Number of elements: %d\n", l->num_cnt);

    /*  Fill new line.  */
    for (int i = 0; i < l->num_cnt; i++) {
        int chrs_rd = sscanf (*input, "%d", &l->nums[i]);

        /*  Testing error cases.  */
        if (chrs_rd == 0) {
            printf ("Error: wrong element\n");
            free_line (l);
            return NULL;
        }
        if (chrs_rd == EOF) {
            printf ("Error: unexpected EOF\n");
            free_line (l);
            return NULL;
        }

        /*  Add whitespace offset. */
        *input += chrs_rd + 1;
    }
    
    return l;
}

void print_matrix (matrix_t *m) {
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

void print_line (line_t *l) {
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