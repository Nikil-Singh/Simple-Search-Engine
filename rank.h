//Nikil Singh

typedef struct rankRep {
    int numFiles;
    struct rankNode *first;
    struct rankNode *last;
} rankRep;

typedef rankRep *rRep;

typedef struct rankNode {
    char *fileName;
    char **links;
    int numLinks;
    struct rankNode *next;
} rankNode;

typedef rankNode *rNode;

#define MAXLINE 1000
#define TRUE       1
#define FALSE      0

//Create new rep for rankRep struct.
rRep newRank();
//Add links to a new node.
void addLinks(rRep R, char *file, FILE *fp);
//Frees up memory assosciated with rankRep struct.
void freeRep(rRep R);
//Print out entire rank struct for testing purposes.
void printRep(rRep R);
//Returns an array with all url links.
char **combineArrays(rRep R);
//Checks if a string already exists within an array of strings.
int notInArray(char *string, char **array);
//Prints out an array of strings.
void printLinks(char **links);
//Returns position/index of a url.
int positionURL(rNode N, char *link);
