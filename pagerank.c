//Nikil Singh

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "list.h"
#include "graph.h"

double *calcPagerank(int maxIterations, double d, double diffPR, uList List, Graph g);
double calcWIN(Graph g, int v, int w);
double calcWOUT(Graph g, int v, int w);
void bubbleSort(double *pagerank, int *urlID, int low, int high);

int main(int argc, char **argv) {
    if (argc != 4) {
        fprintf(stderr, "Not enough valid arguements\n");
        return EXIT_FAILURE;
    }
    double d = atof(argv[1]);
    double diffPR = atof(argv[2]);
    int maxIterations = atoi(argv[3]);
    //Read in data for url files.
    struct urlList *List = readCollection();
    //Initialise a graph for urls.
    Graph web = newGraph(List->numURL);

    //Adds edges to the graph.
    struct urlNode *url;
    int i;
    int v = 0, w = 0;
    for (url = List->first; url != NULL; url = url->next) {
        v = url->urlID;
        for (i = 0; url->urlLinks[i] != NULL; i++) {
            w = searchID(url->urlLinks[i], List);
            if (v == w) continue; //No self links allowed.
            if (w != -1) insertEdge(web, v, w);
        }
    }

    //Calculate pagerank for each url.
    double *Pagerank = calcPagerank(maxIterations, d, diffPR, List, web);

    //Set an array with url id's to keep track of pagerank in sort.
    int *urlID = malloc(sizeof(int)*web->nV);
    assert(urlID != NULL);
    for (i = 0; i < List->numURL; i++) urlID[i] = i;

    //Sorts the pageranks in ascending order.
    bubbleSort(Pagerank, urlID, 0, web->nV);

    FILE *fp = fopen("pagerankList.txt", "w");
    if (fp == NULL) perror("fopen");

    for (i = 0; i < web->nV; i++) {
        char *name = searchName(urlID[i], List);
        if (name == NULL) fprintf(stderr, "No such url exists\n");
        fprintf(fp, "%s, %d, %.7f\n", name, (int)getOutDeg(web, urlID[i]), Pagerank[i]);
    }
    freeGraph(web);
    releaseList(List);
    fclose(fp);
    free(urlID);
    free(Pagerank);
    return 0;
}

double *calcPagerank(int maxIterations, double d, double diffPR, uList List, Graph g) {
    double N = List->numURL;
    int iterations = 0;
    double diff = diffPR;
    double *prevPR = malloc(sizeof(double)*N);
    double *currPR = malloc(sizeof(double)*N);
    int i = 0, j = 0, k = 0;
    double product = 0;

    //Set values for both pagerank arrays.
    for (i = 0; i < N; i++) prevPR[i] = currPR[i] = 1/N;

    //Calculate pagerank for each page.
    while (iterations < maxIterations && diff >= diffPR) {
        //Updates pagerank for each URL.
        for (i = 0; i < N; i++) {
            product = 0;
            for (j = 0; j < N; j++) {
                if (adjacent(g, j, i)) {
                    //Calculate product.
                    product = product + prevPR[j] * calcWIN(g, j, i) * calcWOUT(g, j, i);
                }
            }
            currPR[i] = (1-d)/N + d * product; //Updates pagerank.
        }
        //Update diff.
        for (k = 0; k < N; k++) {
            diff = diff + fabs(currPR[k] - prevPR[k]);
        }
        //Update previous pagerank to current.
        for (k = 0; k < N; k++) prevPR[k] = currPR[k];
        iterations++;
    }
    free(prevPR);
    return currPR;
}

//Calculate weighted incoming links.
double calcWIN(Graph g, int v, int w) {
    //printf("WIN[%d][%d]: %.7f\n", v, w, getInDeg(g, w)/sumInDeg(g, v));
    return getInDeg(g, w)/sumInDeg(g, v);
}

//Calculate weighted outgoing links.
double calcWOUT(Graph g, int v, int w) {
    //printf("WOUT[%d][%d]: %.7f\n", v, w, getOutDeg(g, w)/sumOutDeg(g, v));
    return getOutDeg(g, w)/sumOutDeg(g, v);
}

//Sorts pagerank in ascending order.
void bubbleSort(double *pagerank, int *urlID, int low, int high) {
    int i, j, nswaps, tmpID;
    double tmp;
    for (i = 0; i < high; i++) {
        nswaps = 0;
        for (j = high - 1; j > 0; j--) {
            if (pagerank[j-1] < pagerank[j]) {
                //For Pagerank
                tmp = pagerank[j-1];
                pagerank[j-1] = pagerank[j];
                pagerank[j] = tmp;
                //For Pagerank respective ID's
                tmpID = urlID[j-1];
                urlID[j-1] = urlID[j];
                urlID[j] = tmpID;
                nswaps++;
            }
        }
        if (nswaps == 0) break;
    }
}
