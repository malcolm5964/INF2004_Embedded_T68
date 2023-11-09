//#include "mapping.h"
//
//
//// Create graph
//struct Graph* createGraph(int vertices) {
//  struct Graph* graph = malloc(sizeof(struct Graph));
//  graph->numVertices = vertices;
//
//  graph->adjLists = malloc(vertices * sizeof(struct node*));
//
//  graph->visited = malloc(vertices * sizeof(int));
//
//  int i;
//  for (i = 0; i < vertices; i++) {
//    graph->adjLists[i] = NULL;
//    graph->visited[i] = 0;
//  }
//  return graph;
//}
//
//// Add edge
//void addEdge(struct Graph* graph, int src, int dest) {
//  // Add edge from src to dest
//  struct node* newNode = createNode(dest);
//  newNode->next = graph->adjLists[src];
//  graph->adjLists[src] = newNode;
//
//  // Add edge from dest to src
//  newNode = createNode(src);
//  newNode->next = graph->adjLists[dest];
//  graph->adjLists[dest] = newNode;
//}
//
//void checkWall(struct Graph* graph,int currentvertex) {
//
//    //Add edge if no wall
//    if(checkFrontWall() == 0)
//    {
//        int nextVertex = currentvertex + 1;
//        addEdge(graph, currentvertex, nextVertex);
//    }
//    if(checkLeftWall() == 0)
//    {
//        int nextVertex = currentvertex - 5;
//        if(nextVertex < 0)
//        {
//          nextVertex = nextVertex % 5;
//        }
//        addEdge(graph, currentvertex, nextVertex);
//    }
//    if(checkRightWall() == 0)
//    {
//        int nextVertex = currentvertex + 5;
//        if(nextVertex < 0)
//        {
//          nextVertex = nextVertex % 5;
//        }
//        addEdge(graph, currentvertex, nextVertex);
//    }
//}
//
//void checkdirectionmovement(struct Graph* graph,int currentvertex,int nextvertex){
//  
//}
//
//
//// DFS algo
//void DFS(struct Graph* graph, int vertex) {
//
//  // check direction to add the edge to the map
//  checkWall(graph,vertex);
//
//  //Retrieve adjacency list of current vertex
//  struct node* adjList = graph->adjLists[vertex];
//  //First itme in adjacent list
//  struct node* temp = adjList;
//
//
//  // indicate which vertex(point has been visited)
//  graph->visited[vertex] = 1;
//
//
//  while (temp != NULL) {
//    int connectedVertex = temp->vertex;
//    //current vertex is to track which block on the map that it has to travel
//    //current vertex to show where the robot is at
//    //connected vertex can be track to next block to travel
//    checkdirectionmovement(graph,vertex,connectedVertex);
//
//    if (graph->visited[connectedVertex] == 0) {
//        //recall the funtion with the new current location
//        DFS(graph, connectedVertex);
//
//    }
//    temp = temp->next;
//  }
//
//}
//
//
//
//
//
//void initGraph(int vertex){
//    struct Graph* graph = createGraph(25);
//
//    DFS(graph, 1);
//}