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

void dialog (Dialog_info info, int (*fptr[]) (Graph, FILE*), Graph graph, FILE* file);



/*||||||||||||||||||||||||| < GENERAL FUNCTIONS > |||||||||||||||||||||||||*/

/*  Format of data file: * <vertices> * <edges>  */
int Import (Graph graph, FILE *file);

int Insert (Graph graph, FILE *file);
int InsertVertex (Graph graph, FILE *file);
int InsertEdge (Graph graph, FILE *file);

int Delete (Graph graph, FILE *file);
int DeleteVertex (Graph graph, FILE *file);
int DeleteEdge (Graph graph, FILE *file);

int Update (Graph graph, FILE *file);

int UpdateVertex (Graph graph, FILE *file);
int UpdateVertexName (Graph graph, FILE *file);
int UpdateVertexPort (Graph graph, FILE *file);

int UpdateEdge (Graph graph, FILE *file);
int UpdateEdgePorts (Graph graph, FILE *file);

int Print (Graph graph, FILE *file);



/*||||||||||||||||||||||||| < COMPLEX FUNCTIONS > |||||||||||||||||||||||||*/


int CreateComponents (Graph g, FILE *file);
int DFS (Graph g, FILE *file);
int Djkstra (Graph graph, FILE *file);



