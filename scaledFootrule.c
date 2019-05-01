//Nikil Singh

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <math.h>
#include "rank.h"

void freeArray(char **array);
void calcScaledFootrule(char **links, double *scaledFootrule, rRep R, int i, int *bestPos, int *pos);
int lenArray(char **array);
int positionLinks(char **array, char *string);
void swapInt(int *array, int i, int j);
void printL(char **array);
double calcWCP(rRep R, char **links, int n, int *pos);

int main(int argc, char **argv) {
    //Ensures there are enough arguements entered in the command line.
    if (argc < 2) {fprintf(stderr, "Need to enter more files in command line.\n"); exit(1);}

    //Ensures all files entered are openable.
    int i;
    for (i = 1; i < argc; i++) {
        FILE *fp = fopen(argv[i], "r");
        if (fp == NULL) {perror("fopen"); return EXIT_FAILURE;}
        fclose(fp);
    }

    //Creates struct and adds all respective links to struct.
    struct rankRep *R = newRank();
    for (i = 1; i < argc; i++) {
        FILE *fp = fopen(argv[i], "r");
        addLinks(R, argv[i], fp);
        //fclose(fp); For some reason causes seg fault if actually used.
    }

    //Compiles all links into a single array.
    char **links = combineArrays(R);

    //Sets up arrays and values for calculating W(C,P).
    int *pos = malloc(sizeof(int) * lenArray(links) + 4);
    int *bestPos = malloc(sizeof(int) * lenArray(links) + 4);
    for (i = 0; i < lenArray(links); i++) {pos[i] = i + 1; bestPos[i] = i + 1;}
    double scaledFootrule[1];
    scaledFootrule[0] = 100;

    //Calculate W(C,P)
    calcScaledFootrule(links, scaledFootrule, R, 0, bestPos, pos);

    //Prints out W(C,P)
    printf("%.6f\n", scaledFootrule[0]);

    //Prints out the corresponding links.
    for (i = 0; i < lenArray(links); i++) printf("%s\n", links[bestPos[i]-1]);

    //Frees up memory.
    freeArray(links);
    freeRep(R);
    free(pos);
    free(bestPos);
    return 0;
}

//Frees memory of array of strings.
void freeArray(char **array) {
    int i;
    for (i = 0; array[i] != NULL; i++) free(array[i]);
    free(array);
}

//Calculates the scaled footrule of the links provided.
void calcScaledFootrule(char **links, double *scaledFootrule, rRep R, int i, int *bestPos, int *pos) {
    int j, k;
    if (i == lenArray(links)) {
        //Gets WCP for all files.
        double tmp = calcWCP(R, links, i, pos);

        if (tmp < scaledFootrule[0] && tmp > 0) {
            //Keeps record of bestLinks
            scaledFootrule[0] = tmp;
            for (k = 0; k < i; k++) bestPos[i] = pos[i];
        }
        return;
    }
    for (j = i; j < lenArray(links); j++) {
        //Swaps array positions
        swapInt(pos, i, j);
        calcScaledFootrule(links, scaledFootrule, R, i+1, bestPos, pos);
        //Swaps array positions
        swapInt(pos, i, j);
    }
    return;
}

//Returns length of array of strings.
int lenArray(char **array) {
    int i = 0;
    while (array[i] != NULL) i++;
    return i;
}

//Returns position of url in array.
int positionLinks(char **array, char *string) {
    int i;
    for (i = 0; array[i] != NULL; i++) {
        if (strcmp(array[i], string) == 0) return i;
    }
    return -1;
}

//Switches the values within an array for two given positions.
void swapInt(int *array, int i, int j) {
    int tmp = array[i];
    array[i] = array[j];
    array[j] = tmp;
}

//For testing, prints out an array of strings.
void printL(char **array) {
    int i;
    for (i = 0; array[i] != NULL; i++) printf("%s ", array[i]);
    printf("\n");
}

//Calculate the W(C,P).
double calcWCP(rRep R, char **links, int n, int *pos) {
    double tmp = 0;
    double tmpList = 0;
    double tmpPos = 0;
    struct rankNode *current;
    int j;

    //For each file.
    for (current = R->first; current != NULL; current = current->next) {
        //For each link within union.
        for (j = 0; j < current->numLinks; j++) {
            //If url does not exist in file contiue to next link.
            if (notInArray(links[j], current->links)) continue;
            //Calculates data for the file.
            tmpList = (double)(j) / (double)(current->numLinks);
            //Calculates data for the positioning.
            tmpPos = (double)(pos[j]) / (double)(n);
            //Calculates and increments W(C,P).
            tmp = tmp + fabs(tmpList - tmpPos);
        }
    }
    return tmp;
}
