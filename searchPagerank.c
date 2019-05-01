//Nikil Singh

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include "list.h"

typedef struct searchRes {
    int numQueries;
    struct searchNode *first;
    struct searchNode *last;
} searchRes;

typedef searchRes *searchR;

typedef struct searchNode {
    char *query;                //Holds search item.
    char **links;               //Holds links with search item.
    struct searchNode *next;
} searchNode;

typedef searchNode *searchN;

searchR newRes();
void newSearch(char *query, searchR Res);
void printTestSearchRes(searchR Res);
void freeSearch(searchR Res);
void getLinks(char *query, searchR Res);
char **combineLinks(searchR Res);
int inArrayFalse(char **array, char *string);
void freeArray(char **array);
char **comparePagerank(char **links);
void printLink(char **links);

int main(int argc, char **argv) {
    //Ensures a search query is actually entered.
    if (argc < 2) fprintf(stderr, "Need to enter a search query.\n");

    //Ensures pagerankList.txt and invertedIndex.txt exists.
    FILE *f1 = fopen("pagerankList.txt", "r");
    if (f1 == NULL) {perror("fopen"); return EXIT_FAILURE;}
    fclose(f1);

    FILE *f2 = fopen("invertedIndex.txt", "r");
    if (f2 == NULL) {perror("fopen"); return EXIT_FAILURE;}
    fclose(f2);

    struct searchRes *Res = newRes();
    //Read in search results.
    int i;
    for (i = 1; i < argc; i++) {
        char *query = processText(argv[i]);
        newSearch(query, Res);
    }
    //Get all links between all search terms.
    char **links = combineLinks(Res);
    //Get links in order based on pagerank.
    char **printableLinks = comparePagerank(links);
    //Print out links.
    printLink(printableLinks);

    freeSearch(Res);
    freeArray(links);
    freeArray(printableLinks);
    return 0;
}


//Creates rep for searchRes struct
searchR newRes() {
    struct searchRes *new = malloc(sizeof(struct searchRes));
    assert(new != NULL);
    new->numQueries = 0;
    new->first = new->last = NULL;
    return new;
}

//Adds a new node to search results for a query.
void newSearch(char *query, searchR Res) {
    struct searchNode *new = malloc(sizeof(struct searchNode));
    assert(new != NULL);
    new->query = malloc(strlen(query) + 1);
    assert(new->query != NULL);
    strcpy(new->query, query);
    new->next = NULL;
    if (Res->first == NULL) {
        Res->first = Res->last = new;
    } else {
        Res->last->next = new;
        Res->last = new;
    }
    Res->numQueries++;
    getLinks(query, Res);
}

//Print out search struct. For testing only.
void printTestSearchRes(searchR Res) {
    struct searchNode *current;
    int i;
    printf("Num Queries is %d\n", Res->numQueries);
    for (current = Res->first; current != NULL; current = current->next) {
        printf("Querie: %s\n", current->query);
        printf("Links:\n");
        for (i = 0; current->links[i] != NULL; i++) printf("%s\n", current->links[i]);
    }
    printf("\n");
}

//Frees up memory assosciated with search results struct.
void freeSearch(searchR Res) {
    struct searchNode *current = Res->first;
    int i = 0;
    //Cycles through nodes.
    while (current != NULL) {
        struct searchNode *tmp = current;
        current = current->next;
        free(tmp->query);
        while (tmp->links[i] != NULL) {free(tmp->links[i]); i++;}
        i = 0;
        free(tmp->links);
        free(tmp);
    }
    free(Res);
}

//Enters the links for all queries into struct.
void getLinks(char *query, searchR Res) {

    struct searchNode *current;
    for (current = Res->first; current != NULL; current = current->next) {
        if (strcmp(current->query, query) == 0) break;
    }
    current->links = malloc(sizeof(char *));
    //Opens file.
    FILE *fp = fopen("invertedIndex.txt", "r");
    char line[MAXLINE];
    int found = FALSE;
    int i = 0;
    while (fgets(line, MAXLINE, fp) != NULL) {
        //If first word in file matches query.
        if (strstr(line, query) != NULL) {
            char *tmp = strstr(line, query);
            while (!isspace(tmp[i])) i++;
            if (strncmp(query, tmp, i) == 0) {
                found = TRUE;
                break;
            }
        }
    }
    fclose(fp);
    //If search term is not found.
    if (found == FALSE) return;

    int numLinks = 0;
    //Creates tmpfile to put line into so can use fscanf which can read word by word.
    FILE *tmp = tmpfile();
    fputs(line, tmp);
    rewind(tmp);
    while (fscanf(tmp, "%s", line) != EOF) {
        if (strcmp(line, query) == 0) continue;
        //Copies link into struct.
        current->links[numLinks] = malloc(strlen(line) + 1);
        strcpy(current->links[numLinks], line);
        numLinks++;
        current->links = realloc(current->links, (numLinks+1) * sizeof(char *));
    }
    current->links[numLinks] = NULL;
    fclose(tmp);
}

//Makes array of all links altogether amongst all
char **combineLinks(searchR Res) {
    //Stores all links.
    char **links = malloc(sizeof(char *));
    int numLinks = 0;
    int i;
    struct searchNode *current;
    //Cycle through queries
    for (current = Res->first; current != NULL; current = current->next) {
        //Get links for each query
        for (i = 0; current->links[i] != NULL; i++) {
            if (numLinks != 0) {
                if (inArrayFalse(links, current->links[i])) {
                    links[numLinks] = malloc(strlen(current->links[i]) + 1);
                    strcpy(links[numLinks], current->links[i]);
                    numLinks++;
                    links = realloc(links, (numLinks + 1) * sizeof(char *));
                    links[numLinks] = NULL;
                }
            } else {
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

//Check if string is in array.
int inArrayFalse(char **array, char *string) {
    int i;
    for (i = 0; array[i] != NULL; i++) {
        if (strcmp(string, array[i]) == 0) {
            return FALSE;
        }
    }
    return TRUE;
}

//Frees memory of array of strings.
void freeArray(char **array) {
    int i;
    for (i = 0; array[i] != NULL; i++) free(array[i]);
    free(array);
}

//Compares pageranks of different urls then sends back sorted list.
char **comparePagerank(char **links) {
    int numLinks = 0;
    int ind = 0;
    char line[MAXLINE];
    int i;
    for (i = 0; links[i] != NULL; i++) numLinks++;
    char **pLinks = malloc((numLinks + 1) * sizeof(char *));

    FILE *fp = fopen("pagerankList.txt", "r");
    while (fscanf(fp, "%s", line) != EOF) {
        line[strlen(line) - 1] = '\0'; //Since last character is a ','.
        if (inArrayFalse(links, line)) continue;
        pLinks[ind] = malloc(strlen(line) + 1);
        strcpy(pLinks[ind], line);
        ind++;
    }
    fclose(fp);
    pLinks[ind] = NULL;
    return pLinks;
}

//Prints out an array of strings.
void printLink(char **links) {
    int i;
    for (i = 0; links[i] != NULL; i++) {
        if (i == 30) break;
        printf("%s\n", links[i]);
    }
}
