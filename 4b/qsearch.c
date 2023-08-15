#include "src/lib_bt.h"
#include "src/lib_bt_func.h"
#include "src/generic.h"
#include <limits.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct val_vector Val_vec;

struct val_vector {
    Val *val_vec;
    size_t sz;
};

Val_vec *get_occurences (BNodePtr tree, Key str_to_find);

Val_vec *new_val_vector (size_t sz);
void print_val_vector (Val_vec *vec);
void free_val_vector (Val_vec *vec);

void find_string (BNodePtr tree);
void print_ll_qsearch (InfoPtr head);



int main (int argc, char **argv) {

    BNodePtr root = NULL;
    char *buf = NULL;
    FILE *data = fopen (argv[1], "r");
    if (!data) {
        perror (buf);
        return 0;
    }

    puts ("File indexing start");
    
    char *cur_line = NULL;
    size_t line_num = 0;
    size_t line_len = 0;
    char *line_pos = 0;

    

    while ((cur_line = get_str (data)) && ++line_num) {

        line_len = strlen (cur_line);
        puts (cur_line);
        
        line_pos = cur_line;
        size_t cur_word_len = 0;

        while (line_pos < cur_line + line_len) { 

            char key [100] = {};
            char val [100] = {};
    
            size_t shift = (size_t) (line_pos - cur_line);

            sprintf (val, "%s: %lu: %lu", argv[1], line_num, shift);

            cur_word_len = strcspn (line_pos, ",. :();!?");

            strncpy (key, line_pos, cur_word_len); 

            puts (key);

            insert_bt (&root, key, val);

            //  eat read word
            line_pos += cur_word_len;
            //  eat rejected symbols
            line_pos += strspn(line_pos, ",. :();!?");
        }

        free_nullify (cur_line);
    }

    fclose (data);

    //  debug
    colored_print_bt (root, NULL);
    printf ("\n");

    puts ("File indexing finished");

    find_string (root);
    
    free_bt (&root);
    return 0;
}


void find_string (BNodePtr tree) {

    char *str_to_find = NULL;
    
    puts ("\nEnter string to find:");

    while ((str_to_find = get_str (stdin))) {
    
        Val_vec *vec = get_occurences (tree, str_to_find);
        print_val_vector (vec);
        free_val_vector (vec);

        free_nullify (str_to_find);

        puts ("Enter string to find:");
    }

    return;
}

Val_vec *get_occurences (BNodePtr tree, Key str_to_find) {

    size_t pos = 0;

    BNodePtr node = find_bt (tree, str_to_find, &pos);
    if (!node) return NULL;

    InfoPtr val_node = node->info[pos]->head;
    size_t vec_sz = node->info[pos]->csize;

    Val_vec *val_vec = new_val_vector (vec_sz);

    for (size_t i = 0; i < vec_sz; i++) {

        val_vec->val_vec[i] = val_node->val;
        val_node = val_node->next;
    }

    return val_vec;
}

Val_vec *new_val_vector (size_t sz) {

    Val_vec *val_vec = calloc (1, sizeof *val_vec);

    val_vec->val_vec = calloc (sz, sizeof *val_vec);
    val_vec->sz = sz;

    return val_vec;
}

void print_val_vector (Val_vec *vec) {

    if (!vec) { 
        puts ("String not found\n");
        return;
    }

    for (size_t i = vec->sz - 1; i < INT_MAX; i--) 
        printf ("%s\n", vec->val_vec[i]);

    printf ("\n");
}

void free_val_vector (Val_vec *vec) {

    if (vec) free_nullify (vec->val_vec);
    free_nullify (vec);
}

void print_ll_qsearch (InfoPtr head) {
    
    InfoPtr node = head;

    while (node) {

        printf (YELLOW("%s\n"), node->val);
        node = node->next;
    } 
}