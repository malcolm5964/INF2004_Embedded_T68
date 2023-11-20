#include "mapping.h"

#define north 1
#define east 2
#define south 3
#define west 4
#define gridSize 25

// Create graph
struct Graph* createGraph(int vertices) {
  struct Graph* graph = malloc(sizeof(struct Graph));

  graph->numVertices = vertices;

  graph->adjLists = malloc(vertices * sizeof(struct node*));

  graph->visited = malloc(vertices * sizeof(int));

  int i;
  for (i = 0; i < vertices; i++) {
    graph->adjLists[i] = NULL;
    graph->visited[i] = 0;
  }
  return graph;
}

// Create a node
struct node* createNode(int v) {
  struct node* newNode = malloc(sizeof(struct node));
  newNode->vertex = v;
  newNode->next = NULL;
  return newNode;
}

// Add edge
void addEdge(struct Graph* graph, int src, int dest) {
  printf("Connected vertex %i with vertex %i\n", src, dest);
  // Add edge from src to dest
  struct node* newNode = createNode(dest);
  newNode->next = graph->adjLists[src];
  graph->adjLists[src] = newNode;

  // Add edge from dest to src
  newNode = createNode(src);
  newNode->next = graph->adjLists[dest];
  graph->adjLists[dest] = newNode;
}

void checkVertex(struct Graph* graph,int currentvertex, int direction) {

    //Move 10cm to check left and right
    move(10, 0);

    //Direction is facing NORTH
    if(direction == north)
    {
      if(left_ir() == 0)
      {
          int nextVertex = currentvertex - 5;
          if(nextVertex < 1)
          {
            nextVertex = nextVertex + gridSize;
          }
          addEdge(graph, currentvertex, nextVertex);
      }
      if(right_ir() == 0)
      {
          int nextVertex = currentvertex + 5;
          if(nextVertex > 25)
          {
            nextVertex = nextVertex - gridSize;
          }
          addEdge(graph, currentvertex, nextVertex);
      }
    }
    //Direction is facing EAST
    if(direction == east)
    {
      if(left_ir() == 0)
      {
          int nextVertex = currentvertex + 1;
          addEdge(graph, currentvertex, nextVertex);
      }
      if(right_ir() == 0)
      {
          int nextVertex = currentvertex - 1;
          addEdge(graph, currentvertex, nextVertex);
      }
    }
    //Direction is facing SOUTH
    if(direction == south)
    {
      if(left_ir() == 0)
      {
          int nextVertex = currentvertex + 5;
          if(nextVertex > 25)
          {
            nextVertex = nextVertex - gridSize;
          }
          addEdge(graph, currentvertex, nextVertex);
      }
      if(right_ir() == 0)
      {
          int nextVertex = currentvertex - 5;
          if(nextVertex < 1)
          {
            nextVertex = nextVertex + gridSize;
          }
          addEdge(graph, currentvertex, nextVertex);
      }
    }
    //Direction is facing WEST
    if(direction == west)
    {
      if(left_ir() == 0)
      {
          int nextVertex = currentvertex - 1;
          addEdge(graph, currentvertex, nextVertex);
      }
      if(right_ir() == 0)
      {
          int nextVertex = currentvertex + 1;
          addEdge(graph, currentvertex, nextVertex);
      }
    }
  
  
    //CHECK FRONT WALL
    move(9, 0);

    printf("front ir: %i\n", front_ir());
    printf("direction: %i\n", direction);

    if(front_ir() == 0)
    {
      if(direction == north)
      {
        int nextVertex = currentvertex + 1;
        addEdge(graph, currentvertex, nextVertex);
      }
      if(direction == east)
      {
        int nextVertex = currentvertex + 5;
        if(nextVertex > 25)
        {
          nextVertex = nextVertex - gridSize;
        }
        addEdge(graph, currentvertex, nextVertex);
      }
      if(direction == south)
      {
        int nextVertex = currentvertex - 1;
        addEdge(graph, currentvertex, nextVertex);
      }
      if(direction == west)
      {
        int nextVertex = currentvertex - 5;
        if(nextVertex < 1)
        {
          nextVertex = nextVertex + gridSize;
        }
        addEdge(graph, currentvertex, nextVertex);
      }                  
    }

}




//Check whether to turn or move forward
int checkdirectionmovement(struct Graph* graph,int currentvertex,int nextvertex, int direction)
{
    if(direction == north)
    {
      //Next Vertex is North
      if(currentvertex+1 == nextvertex)
      {
        return north;
      }

      //Next Vertex is East
      if((currentvertex+5 == nextvertex) || (currentvertex+5 - gridSize == nextvertex))
      {
        move(9, 1);
        turn_right();
        return east;
      }

      //Next Vertex is South
      if(currentvertex - 1 == nextvertex)
      {
        //move car backwards ERROR
        move(19,1);
        return north;
      }

      //Next Vertex is West
      if((currentvertex-5 == nextvertex) || (currentvertex-5 + gridSize == nextvertex))
      {
        move(9, 1);
        turn_left();
        return west;
      }
    }

    if(direction == east)
    {
      //Next Vertex is North
      if(currentvertex + 1 == nextvertex)
      {
        move(9,1);
        turn_left();
        return north;
      }
      //Next Vertex is East
      {
        if((currentvertex + 5 == nextvertex) || (currentvertex+5 - gridSize == nextvertex))
        {
          return east;
        }
      }
      //Next Vertex is South
      if(currentvertex - 1 == nextvertex)
      {
        move(9,1);
        turn_left();
        return south;
      }
      //Next Vertex West
      if((currentvertex-5 == nextvertex) || (currentvertex-5 + gridSize == nextvertex))
      {
        move(19,1);
        return east;
      }
    }

    if(direction == south)
    {
      //Next Vertex is North
      if(currentvertex + 1 == nextvertex)
      {
        move(19,1);
        return south;
      }
      //Next Vertex is East
      if((currentvertex + 5 == nextvertex) || (currentvertex+5 - gridSize == nextvertex))
      {
        move(9,1);
        turn_left();
        return east;
      }
      //Next Vertex is South
      if(currentvertex - 1 == nextvertex)
      {
        return south;
      }
      //Next Vertex is West
      if((currentvertex-5 == nextvertex) || (currentvertex-5 + gridSize == nextvertex))
      {
        move(9,1);
        turn_right();
        return west;
      }
    }

    if(direction == west)
    {
      //Next Vertex is North
      if(currentvertex + 1 == nextvertex)
      {
        move(9,1);
        turn_right();
        return north;
      }
      //Next Vertex is East
      if((currentvertex + 5 == nextvertex) || (currentvertex+5 - gridSize == nextvertex))
      {
        move(19,1);
        return west;
      }
      //Next Vertex is South
      if(currentvertex - 1 == nextvertex)
      {
        move(9,1);
        turn_left();
        return south;
      }
      //Next Vertex is West
      if((currentvertex-5 == nextvertex) || (currentvertex-5 + gridSize == nextvertex))
      {
        return west;
      }
    }

}


// DFS algo
void DFS(struct Graph* graph, int vertex, int direction) {

  printf("Checking vertex: %i\n", vertex);
  // check direction to add the edge to the map
  checkVertex(graph,vertex, direction);

  
  vTaskDelay(5000);

  //Retrieve adjacency list of current vertex
  struct node* adjList = graph->adjLists[vertex];
  //First itme in adjacent list
  struct node* temp = adjList;


  // indicate which vertex(point has been visited)
  graph->visited[vertex] = 1;


  while (temp != NULL) {
    int connectedVertex = temp->vertex;
    //current vertex is to track which block on the map that it has to travel
    //current vertex to show where the robot is at
    //connected vertex can be track to next block to travel
    printf("test\n");
    int newDirection = checkdirectionmovement(graph, vertex, connectedVertex, direction);

    if (graph->visited[connectedVertex] == 0) {
        //recall the funtion with the new current location
        DFS(graph, connectedVertex, newDirection);

    }
    temp = temp->next;
  }

}





void initGraph(){
    struct Graph* graph = createGraph(gridSize);

    DFS(graph, 1, north);

    printf("Map done\n");
}