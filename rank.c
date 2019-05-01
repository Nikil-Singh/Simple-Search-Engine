//Nikil Singh

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <math.h>
#include "rank.h"

//Create new rep for rankRep struct.
rRep newRank() {
    struct rankRep *new = malloc(sizeof(struct rankRep));
    assert(new != NULL);
    new->first = new->last = NULL;
    new->numFiles = 0;
    return new;
}

//Add links to a new node.
void addLinks(rRep R, char *file, FILE *fp) {
    struct rankNode *new = malloc(sizeof(struct rankNode));
    assert(new != NULL);
    new->fileName = malloc(strlen(file) + 1);
    assert(new->fileName != NULL);
    strcpy(new->fileName, file);
    //Cycle through lines in files and add to links for struct.
    new->links = malloc(sizeof(char *));
    assert(new->links != NULL);
    new->numLinks = 0;
    char line[MAXLINE];
    //FILE *fp = fopen(file, "r");
    while (fscanf(fp, "%s", line) != EOF) {
        new->links[new->numLinks] = malloc(strlen(line) + 1);
        strcpy(new->links[new->numLinks], line);
        new->numLinks++;
        new->links = realloc(new->links, (new->numLinks + 1) * sizeof(char *));
    }
    new->links[new->numLinks] = NULL;
    if (R->first == NULL) {
        R->first = R->last = new;
    } else {
        R->last->next = new;
        R->last = new;
    }
    R->numFiles++;
}

//Frees up memory assosciated with rankRep struct.
void freeRep(rRep R) {
    struct rankNode *current = R->first;
    int i = 0;
    while (current != NULL) {
        struct rankNode *tmp = current;
        current = current->next;
        free(tmp->fileName);
        while (tmp->links[i] != NULL) {free(tmp->links[i]); i++;}
        i = 0;
        free(tmp->links);
        free(tmp);
    }
    free(R);
}

//Print out entire rank struct for testing purposes.
void printRep(rRep R) {
    struct rankNode *current;
    int i;
    printf("Num Files: %d\n", R->numFiles);
    for (current = R->first; current != NULL; current = current->next) {
        printf("Filename: %s\n", current->fileName);
        printf("Num Links in %s is %d\n", current->fileName, current->numLinks);
        for (i = 0; current->links[i] != NULL; i++) printf("Link: %s\n", current->links[i]);
    }
}

//Returns an array with all url links.
char **combineArrays(rRep R) {
    //Stores all links.
    char **links = malloc(sizeof(char *));
    int numLinks = 0;
    int i = 0;
    struct rankNode *current;
    //Cycle through queries
    for (current = R->first; current != NULL; current = current->next) {
        //Get links for each query
        for (i = 0; current->links[i] != NULL; i++) {
            if (notInArray(current->links[i], links)) {
                links[numLinks] = malloc(strlen(current->links[i]) + 1);
                strcpy(links[numLinks], current->links[i]);
                numLinks++;
                links = realloc(links, (numLinks + 1) * sizeof(char *));
                links[numLinks] = NULL;
            }
        }
    }
    links[numLinks] = NULL;
    return links;
}

//Checks if a string already exists within an array of strings.
int notInArray(char *string, char **array) {
    int i;
    for (i = 0; array[i] != NULL; i++) {
        if (strcmp(string, array[i]) == 0) {
            return FALSE;
        }
    }
    return TRUE;
}

//Prints out an array of strings.
void printLinks(char **links) {
    int i;
    for (i = 0; links[i] != NULL; i++) {
        printf("%s\n", links[i]);
    }
}

//Returns position/index of a url.
int positionURL(rNode N, char *link) {
    int i;
    for (i = 0; N->links[i] != NULL; i++) {
        if (strcmp(N->links[i], link) == 0) return i;
    }
    return -1;
}
