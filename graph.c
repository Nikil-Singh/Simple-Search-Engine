//Nikil Singh

//Adapted from lab ADT.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "graph.h"

//Creats a new graph that is empty.
Graph newGraph(int nV) {
    assert(nV > 0);
	int v, w;
	Graph new = malloc(sizeof(GraphRep));
	assert(new != 0);
	new->nV = nV; new->nE = 0;
	new->edges = malloc(nV*sizeof(int *));
	assert(new->edges != 0);
	for (v = 0; v < nV; v++) {
		new->edges[v] = malloc(nV*sizeof(int));
		assert(new->edges[v] != 0);
		for (w = 0; w < nV; w++)
			new->edges[v][w] = 0;
	}
	return new;
}

//Display the graph.
void showGraph(Graph g) {
    assert(g != NULL);
	printf("\n#vertices=%d, #edges=%d\n\n",g->nV,g->nE);
	int v, w;
    //Prints out the adjacency matrix.
    printf("  ");
    for (v = 0; v < g->nV; v++) printf("%d ", v);
    printf("\n");
	for (v = 0; v < g->nV; v++) {
		printf("%d ",v);
		for (w = 0; w < g->nV; w++) {
			printf("%d ",g->edges[v][w]);
		}
		printf("\n");
	}
}

//Insert edge into graph.
void insertEdge(Graph g, int vertexV, int vertexW) {
    assert(g != NULL && validV(g,vertexV) && validV(g,vertexW ));
	if (g->edges[vertexV][vertexW] == 0) {
		g->edges[vertexV][vertexW] = 1;
		g->nE++;
	}
}

// Ensure vertex is valid.
int validV(Graph g, int vertex) {
    return (g != NULL && vertex >= 0 && vertex < g->nV);
}

//Frees memory assosciated with the graph.
void freeGraph(Graph g) {
    assert (g != NULL);
    int i;
    for (i = 0; i < g->nV; i++) free(g->edges[i]);
    free(g->edges);
    free(g);
}

//Checks if to verticies are adjacent.
int adjacent(Graph g, int vertexV, int vertexW) {
    assert(g != NULL);
    return g->edges[vertexV][vertexW];
}

//Returns the sum of out degrees.
double sumOutDeg(Graph g, int v) {
    assert(g != NULL);
    double sum = 0;
    int i;
    for (i = 0; i < g->nV; i++) {
        if (adjacent(g, v, i)) sum = sum + getOutDeg(g, i);
    }
    return sum;
}

//Returns the sum of in degrees.
double sumInDeg(Graph g, int v) {
    assert(g != NULL);
    double sum = 0;
    int i;
    for (i = 0; i < g->nV; i++) {
        if (adjacent(g, v, i)) sum = sum + getInDeg(g, i);
    }
    return sum;
}

//Returns the out degree for a vertex.
double getOutDeg(Graph g, int v) {
    assert(g != NULL);
    double outDeg = 0;
    int i;
    for (i = 0; i < g->nV; i++) {
        if (adjacent(g, v, i)) outDeg++;
    }
    if (outDeg == 0) outDeg = 0.5;
    return outDeg;
}

//Returns the in degree for a vertex.
double getInDeg(Graph g, int v) {
    assert(g != NULL);
    double inDeg = 0;
    int i;
    for (i = 0; i < g->nV; i++) {
        if (adjacent(g, i, v)) inDeg++;
    }
    return inDeg;
}
