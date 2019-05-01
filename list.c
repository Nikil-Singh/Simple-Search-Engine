//Nikil Singh

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include "list.h"

#define FILESIZE    10
#define MAXLINE   1000
#define MAXURLN     10

uList readCollection() {
    char *url = malloc(MAXLINE);
    assert(url != NULL);
    //Initialises a list to store url's.
    struct urlList *uList = newList();

    //Opens collection.txt file for url's.
    FILE *fp = fopen("collection.txt", "r");
    if (fp == NULL) perror("fopen");

    //Scans the contents of the collection.txt file into list.
    while (fscanf(fp, "%s", url) != EOF) {
        newNode(url, uList);
    }
    fclose(fp);
    free(url);
    return uList;
}

//Reads from urlXX.txt files and stores data in list.
void readURLFile(url Node, char *urlFile) {
    char urlF[FILESIZE];
    char line[MAXLINE];
    int i = 0, j = 0, k = 0;
    //Node->urlLinks = malloc(strlen("a"));
    //assert(Node->urlLinks != NULL);
    sprintf(urlF, "%s.txt", urlFile);

    FILE *fp = fopen(urlF, "r");
    if (fp == NULL) perror("fopen");

    while (fscanf(fp, "%s", line) != EOF) {
        if (line[0] == '\n' || line[0] == '\0') continue;
        //j = 0 for start of section 1, j = 2 for start of section 2.
        if (strcmp(line, "#start") == 0 && (j == 0 || j == 2)) {j++; continue;}
        //j = 1 for end of section 1, j = 3 for end of section 2.
        if (strcmp(line, "#end") == 0 && (j == 1 || j == 3)) {j++; continue;}
        //If word is the following then skip.
        if (strcmp(line, "Section-1") == 0 || strcmp(line, "Section-2") == 0) continue;

        if (j == 1) {
            //Adds urls to array.
            Node->urlLinks[i] = malloc(strlen(line) + 1);
            strcpy(Node->urlLinks[i], line);
            i++;
            Node->urlLinks = realloc(Node->urlLinks, (i+1) * sizeof(char *));
            continue;
        } else if (j == 3) {
            //Collects content.
            char *word = processText(line);
            Node->contents[k] = malloc(strlen(word) + 1);
            strcpy(Node->contents[k], word);
            k++;
            Node->contents = realloc(Node->contents, (k+1) * sizeof(char *));
            continue;
        }
    }
    Node->urlLinks[i] = NULL;
    Node->contents[k] = NULL;

    fclose(fp);
}

//Creates a new list from collection.txt
uList newList() {
    struct urlList *new = malloc(sizeof(struct urlList));
    assert(new != NULL);
    new->numURL = 0;
    new->first = new->last = NULL;
    return new;
}

//Creates node for a particular urlXX.txt file.
void newNode(char *url, uList List) {
    struct urlNode *new = malloc(sizeof(struct urlNode));
    assert(new != NULL);
    new->urlID = List->numURL;
    new->url = malloc(strlen(url) + 1);
    assert(new->url != NULL);
    strcpy(new->url, url);
    new->next = NULL;
    if (List->first == NULL) {
        List->first = List->last = new;
    } else {
        List->last->next = new;
        List->last = new;
    }
    List->numURL++;
    new->urlLinks = malloc(sizeof(char *));
    assert(new->urlLinks != NULL);
    new->contents = malloc(sizeof(char *));
    assert(new->contents != NULL);
    readURLFile(new, new->url);
}

//Print out list url's.
void printList(uList List) {
    struct urlNode *current;
    int i = 0;
    printf("Num URLs: %d\n\n", List->numURL);
    printf("-----------------\n");
    for (current = List->first; current != NULL; current = current->next) {
        printf("URL: %s\n", current->url);
        printf("-----------------\n");
        printf("Content:\n");
        for (i = 0; current->contents[i] != NULL; i++) printf("%s ", current->contents[i]);
        printf("\n-----------------\n");
        printf("URL Links:\n");
        for (i = 0; current->urlLinks[i] != NULL; i++) printf("%s\n", current->urlLinks[i]);
        printf("-----------------\n");
    }
}

//Print out IDs.
void printListIDs(uList List) {
    struct urlNode *current;
    for (current = List->first; current != NULL; current = current->next) {
        printf("%s has ID: %d\n", current->url, current->urlID);
    }
}

//Returns pointer for a certain url.
url searchURL(char *url, uList List) {
    struct urlNode *new;
    //Cycles through list to find url.
    for (new = List->first; new != NULL; new = new->next) {
        if (strcmp(url, new->url) == 0) return new;
    }
    return NULL;
}

//Returns ID number for url.
int searchID(char *url, uList List) {
    struct urlNode *new;
    //Cycles through list to find url.
    for (new = List->first; new != NULL; new = new->next) {
        if (strcmp(url, new->url) == 0) return new->urlID;
    }
    return -1;
}

//Returns url name by id.
char *searchName(int ID, uList List) {
    struct urlNode *new;
    //Cycles through list to find url.
    for (new = List->first; new != NULL; new = new->next) {
        if (ID == new->urlID) return new->url;
    }
    return NULL;
}

//Remove specific characters.
char *processText(char *contents) {
    int i = 0, j = 0;
    for (i = 0; i < strlen(contents); i++) {
        contents[i] = tolower(contents[i]);
        if (i >= 1 && i <= strlen(contents) - 2) {
            if (isalnum(contents[i-1]) && isalnum(contents[i+1]) && (!isalnum(contents[i]) && !isspace(contents[i]) && contents[i] != '\0')) {
                continue;
            }
        }
        if (!isalnum(contents[i]) && !isspace(contents[i]) && contents[i] != '\0') {
            for (j = i; j < strlen(contents); j++) contents[j] = contents[j+1];
        }
    }
    return contents;
}

//Trims of leading and trailing spaces.
void trim(char *str) {
    int first, last;
    first = 0;
    while (isspace(str[first])) first++;
    last  = strlen(str)-1;
    while (isspace(str[last])) last--;
    int i, j = 0;
    for (i = first; i <= last; i++) str[j++] = str[i];
    str[j] = '\0';
}

//Frees up memory for List.
void releaseList(uList List) {
    struct urlNode *current = List->first;
    int i = 0;
    //Cycles through list to free up memory assosciated with each node.
    while (current != NULL) {
        struct urlNode *tmp = current;
        current = current->next;
        free(tmp->url);
        while (tmp->urlLinks[i] != NULL) {free(tmp->urlLinks[i]); i++;}
        i = 0;
        while (tmp->contents[i] != NULL) {free(tmp->contents[i]); i++;}
        i = 0;
        free(tmp->urlLinks);
        free(tmp->contents);
        free(tmp);
    }
    free(List);
}
