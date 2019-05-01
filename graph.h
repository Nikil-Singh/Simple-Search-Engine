//Nikil Singh

//Adapted from lab ADT.

// graph representation (adjacency matrix)
typedef struct GraphRep {
	int    nV;    // #vertices
	int    nE;    // #edges
	int  **edges; // matrix of weights (0 == no edge)
} GraphRep;

typedef struct GraphRep *Graph;

//Creats a new graph that is empty.
Graph newGraph(int nV);
//Display the graph.
void showGraph(Graph g);
//Insert edge into graph.
void insertEdge(Graph g, int vertexV, int vertexW);
//Ensure vertex is valid.
int validV(Graph g, int vertex);
//Frees memory assosciated with the graph.
void freeGraph(Graph g);
//Checks if to verticies are adjacent.
int adjacent(Graph g, int vertexV, int vertexW);
//Returns the sum of out degrees.
double sumOutDeg(Graph g, int v);
//Returns the sum of in degrees.
double sumInDeg(Graph g, int v);
//Returns the out degree for a vertex.
double getOutDeg(Graph g, int v);
//Returns the in degree for a vertex.
double getInDeg(Graph g, int v);
