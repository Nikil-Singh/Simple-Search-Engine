//Nikil Singh

typedef struct urlList {
    int numURL;
    struct urlNode* first;
    struct urlNode* last;
} urlList;

typedef urlList *uList;

typedef struct urlNode {
    int  urlID;          //ID number for url to enter in graph, starts from 0.
    char *url;            //URL file number
    char **urlLinks;      //All outgoing links from one page.
    char **contents;     //All the content from the page processed.
    struct urlNode *next;
} urlNode;

typedef urlNode *url;

#define MAXLINE    1000
#define TRUE          1
#define FALSE         0
#define NONE         -1
#define URLFILE       9

//Reads from the collection.txt file and creates a new list.
uList readCollection();
//Reads from urlXX.txt files and stores data in list.
void readURLFile(url Node, char *urlFile);
//Creates a new list from collection.txt
uList newList();
//Creates node for a particular urlXX.txt file.
void newNode(char *url, uList List);
//Print out list url's.
void printList(uList List);
//Print out IDs.
void printListIDs(uList List);
//Returns pointer for a certain url.
url searchURL(char *url, uList List);
//Returns ID number for url.
int searchID(char *url, uList List);
//Returns url name by id.
char *searchName(int ID, uList List);
//Remove specific characters.
char *processText(char *contents);
//Trims of leading and trailing spaces.
void trim(char *str);
//Frees up memory for List.
void releaseList(uList List);
