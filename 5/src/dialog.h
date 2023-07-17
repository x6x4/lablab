#pragma once

#include "lib_graph.h"
#include <stddef.h>
#include <stdio.h>


/*  DIALOG  */

typedef struct dialog_strings *Dialog_info;

struct dialog_strings {
    const char *greeting;
    const char *msgs;
    size_t msgc;
    const char *exit_msg;
};

void dialog (Dialog_info info, Graph graph, FILE* file);

/*||||||||||||||||||||||||| < GENERAL FUNCTIONS > |||||||||||||||||||||||||*/


int Insert (Graph graph, FILE *file);
int InsertVertex (Graph graph, FILE *file);
int InsertEdge (Graph graph, FILE *file);

int Delete (Graph graph, FILE *file);
int DeleteVertex (Graph graph, FILE *file);
int DeleteEdge (Graph graph, FILE *file);

int Update (Graph graph, FILE *file);
int UpdateVertex (Graph graph, FILE *file);
int UpdateEdge (Graph graph, FILE *file);

int Print (Graph graph, FILE *file);



/*||||||||||||||||||||||||| < COMPLEX FUNCTIONS > |||||||||||||||||||||||||*/

int Find (Graph graph, FILE *file);
int Trav (Graph graph, FILE *file);

int import_tree (Graph graph, FILE *file);

