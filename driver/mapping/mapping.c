#include "mapping.h"

#define north 1
#define east 2
#define south 3
#define west 4
#define gridSize 36
#define gridWidth 6

char* mazeHTML;
char *mazeHTML1;
char *mazeHTML2;
char *mazeHTML3;
char *mazeHTML4;

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

    //Direction is facing NORTH
    if(direction == north)
    {
      if(left_ir() == 0)
      {
          int nextVertex = currentvertex - gridWidth;
          if(nextVertex < 1)
          {
            nextVertex = nextVertex + gridSize;
          }
          addEdge(graph, currentvertex, nextVertex);
      }
      if(right_ir() == 0)
      {
          int nextVertex = currentvertex + gridWidth;
          if(nextVertex > gridSize)
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
          int nextVertex = currentvertex + gridWidth;
          if(nextVertex > gridSize)
          {
            nextVertex = nextVertex - gridSize;
          }
          addEdge(graph, currentvertex, nextVertex);
      }
      if(right_ir() == 0)
      {
          int nextVertex = currentvertex - gridWidth;
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
  
    //Check Front Wall
    if(front_ir() == 0)
    {
      if(direction == north)
      {
        int nextVertex = currentvertex + 1;
        addEdge(graph, currentvertex, nextVertex);
      }
      if(direction == east)
      {
        int nextVertex = currentvertex + gridWidth;
        if(nextVertex > gridSize)
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
        int nextVertex = currentvertex - gridWidth;
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
        move(19, 0);
        return north;
      }

      //Next Vertex is East
      if((currentvertex+gridWidth == nextvertex) || (currentvertex+gridWidth - gridSize == nextvertex))
      {
        turn_right();
        move(19,0);
        return east;
      }

      //Next Vertex is South
      if(currentvertex - 1 == nextvertex)
      {
        //move car backwards ERROR
        turn_right();
        turn_right();
        move(19,0);
        return south;
      }

      //Next Vertex is West
      if((currentvertex-gridWidth == nextvertex) || (currentvertex-gridWidth + gridSize == nextvertex))
      {
        turn_left();
        move(19,0);
        return west;
      }
    }

    if(direction == east)
    {
      //Next Vertex is North
      if(currentvertex + 1 == nextvertex)
      {
        turn_left();
        move(19,0);
        return north;
      }
      //Next Vertex is East
      {
        if((currentvertex + gridWidth == nextvertex) || (currentvertex+gridWidth - gridSize == nextvertex))
        {
          move(19,0);
          return east;
        }
      }
      //Next Vertex is South
      if(currentvertex - 1 == nextvertex)
      {
        turn_right();
        move(19,0);
        return south;
      }
      //Next Vertex West
      if((currentvertex-gridWidth == nextvertex) || (currentvertex-gridWidth + gridSize == nextvertex))
      {
        turn_left();
        turn_left();
        move(19,0);
        return west;
      }
    }

    if(direction == south)
    {
      //Next Vertex is North
      if(currentvertex + 1 == nextvertex)
      {
        turn_right();
        turn_right();
        move(19,0);
        return north;
      }
      //Next Vertex is East
      if((currentvertex + gridWidth == nextvertex) || (currentvertex+gridWidth - gridSize == nextvertex))
      {
        turn_left();
        move(19,0);
        return east;
      }
      //Next Vertex is South
      if(currentvertex - 1 == nextvertex)
      {
        move(19,0);
        return south;
      }
      //Next Vertex is West
      if((currentvertex-gridWidth == nextvertex) || (currentvertex-gridWidth + gridSize == nextvertex))
      {
        turn_right();
        move(19,0);
        return west;
      }
    }

    if(direction == west)
    {
      //Next Vertex is North
      if(currentvertex + 1 == nextvertex)
      {
        turn_right();
        move(19,0);
        return north;
      }
      //Next Vertex is East
      if((currentvertex + gridWidth == nextvertex) || (currentvertex+gridWidth - gridSize == nextvertex))
      {
        turn_right();
        turn_right();
        move(19,0);
        return east;
      }
      //Next Vertex is South
      if(currentvertex - 1 == nextvertex)
      {
        turn_left();
        move(19,0);
        return south;
      }
      //Next Vertex is West
      if((currentvertex-gridWidth == nextvertex) || (currentvertex-gridWidth + gridSize == nextvertex))
      {
        move(19,0);
        return west;
      }
    }

}


// DFS algo
void DFS(struct Graph* graph, int vertex, int direction) {

  printf("Checking vertex: %i\n", vertex);
  // check direction to add the edge to the map
  checkVertex(graph,vertex, direction);

  //Retrieve adjacency list of current vertex
  struct node* adjList = graph->adjLists[vertex];
  //First itme in adjacent list
  struct node* temp = adjList;


  // indicate which vertex(point has been visited)
  graph->visited[vertex] = 1;


  while (temp != NULL) {
    int connectedVertex = temp->vertex;

    //Position the car to the center
    int newDirection = checkdirectionmovement(graph, vertex, connectedVertex, direction);

    if (graph->visited[connectedVertex] == 0) {
        //recall the funtion with the new current location
        DFS(graph, connectedVertex, newDirection);

    }
    temp = temp->next;
  }

}


void drawMap(struct Graph* graph){
  uint8_t number = 1;
  const int bufferSize = 1024;

  mazeHTML1 = (char*)malloc(bufferSize * sizeof(char));
  mazeHTML1[0] = '\0';
  mazeHTML2 = (char*)malloc(bufferSize * sizeof(char));
  mazeHTML2[0] = '\0';
  mazeHTML3 = (char*)malloc(bufferSize * sizeof(char));
  mazeHTML3[0] = '\0';
  mazeHTML4 = (char*)malloc(bufferSize * sizeof(char));
  mazeHTML4[0] = '\0';
  

  //Row Level HTML
  for (int j = 0; j < gridWidth; j++) {

    int startHead = 1 + gridWidth * j;

    struct node* adjList = graph->adjLists[startHead];
    struct node* temp = adjList;

    if(temp != NULL) {
      //Column Level HTML
      if(number == 1){
        mazeHTML = mazeHTML1;
      }
      else if(number == 2){
        mazeHTML = mazeHTML2;
      }
      else if(number == 3){
        mazeHTML = mazeHTML3;
      }
      else if(number == 4){
        mazeHTML = mazeHTML4;
      }
      //snprintf(mazeHTML + strlen(mazeHTML), bufferSize - strlen(mazeHTML), "<tr>");

        for (int i = startHead; i < startHead+6; i++) {
          snprintf(mazeHTML + strlen(mazeHTML), bufferSize - strlen(mazeHTML), "<td class='");
          struct node* adjList = graph->adjLists[i];
          struct node* temp = adjList;
      
          while(temp != NULL) {
            int connectedVertex = temp->vertex;

            //Add right
            if(i + 1 == connectedVertex) {
              snprintf(mazeHTML + strlen(mazeHTML), bufferSize - strlen(mazeHTML), "right ");
            }
            //Add left
            if(i - 1 == connectedVertex) {
              snprintf(mazeHTML + strlen(mazeHTML), bufferSize - strlen(mazeHTML), "left ");
            }
            //Add top
            if(i - 6 == connectedVertex || i - 6 + 36 == connectedVertex) {
              snprintf(mazeHTML + strlen(mazeHTML), bufferSize - strlen(mazeHTML), "top ");
            }
            //Add bottom
            if(i + 6 == connectedVertex || i + 6 - 36 == connectedVertex) {
              snprintf(mazeHTML + strlen(mazeHTML), bufferSize - strlen(mazeHTML), "bottom ");
            }
            temp = temp->next;
          }
        snprintf(mazeHTML + strlen(mazeHTML), bufferSize - strlen(mazeHTML), "'></td>");
        }

        //snprintf(mazeHTML + strlen(mazeHTML), bufferSize - strlen(mazeHTML), "</tr>");
        printf("\nHTML %s\n", mazeHTML);
        number++;
    }
    else {
      continue;
    }

  }
}

//QUEUE FUNCTIONS
// Create a queue
struct queue* createQueue() {
  struct queue* q = malloc(sizeof(struct queue));
  q->front = -1;
  q->rear = -1;
  return q;
}

// Check if the queue is empty
int isEmpty(struct queue* q) {
  if (q->rear == -1)
    return 1;
  else
    return 0;
}

// Adding elements into queue
void enqueue(struct queue* q, int value) {
  if (q->rear == SIZE - 1)
    printf("\nQueue is Full!!");
  else {
    if (q->front == -1)
      q->front = 0;
    q->rear++;
    q->items[q->rear] = value;
  }
}

// Removing elements from queue
int dequeue(struct queue* q) {
  int item;
  if (isEmpty(q)) {
    printf("Queue is empty");
    item = -1;
  } else {
    item = q->items[q->front];
    q->front++;
    if (q->front > q->rear) {
      //printf("Resetting queue ");
      q->front = q->rear = -1;
    }
  }
  return item;
}

// Print the queue
void printQueue(struct queue* q) {
  int i = q->front;

  if (isEmpty(q)) {
    printf("Queue is empty");
  } else {
    printf("\nQueue contains \n");
    for (i = q->front; i < q->rear + 1; i++) {
      printf("%d \n", q->items[i]);
    }
  }
}



int bfs(struct Graph* graph, int start, int end, int prev[]) {
  struct queue* q = createQueue();

  int visited[36];

  for (int i = 0; i < 37; i++) {
    visited[i] = 0;
    prev[i] = -1;
  }

  visited[start] = 1;

  enqueue(q, start);

  while (!isEmpty(q)) {
    
    int current = dequeue(q);
    struct node* temp = graph->adjLists[current];

    while(temp) {
      int adjVertex = temp->vertex;

      //Check if visited
      if (visited[adjVertex] == 0) {
        visited[adjVertex] = 1;

        prev[adjVertex] = current;

        enqueue(q, adjVertex);

        if (current == end) {
          return 1;
        }
      }
      temp = temp->next;
    }
  }
  return 0;
}



// BFS algorithm
void shortestPath(struct Graph* graph, int start, int end) {
  int prev[36];

  if(!bfs(graph, start, end, prev)) {
    printf("Not connected to end");
    return;
  }

  printf("\nFinding Shortest Path");
  
  struct queue* shortestPath = createQueue();

  int prevVertex = end;
  
  enqueue(shortestPath, prevVertex);

  while(prev[prevVertex] != -1) {
    enqueue(shortestPath, prev[prevVertex]);
    prevVertex = prev[prevVertex];
  }

  printQueue(shortestPath);
}


void initGraph(){
    struct Graph* graph = createGraph(gridSize);

    //DFS(graph, 1, north);

    addEdge(graph, 1, 0);

    //Car at 1
    addEdge(graph, 1, 2);

    addEdge(graph, 1, 7);

    addEdge(graph, 1, 31);

    //Car at 31
    addEdge(graph, 31, 32);

    addEdge(graph, 31, 25);

    //Car at 32
    addEdge(graph, 32, 26);

    addEdge(graph, 32, 33);

    //Car at 33
    addEdge(graph, 33, 3);

    //Car at 3
    addEdge(graph, 3, 9);

    //Car at 9
    addEdge(graph, 9, 8);

    //Car at 26
    addEdge(graph, 26, 27);

    //Car at 27
    addEdge(graph, 27, 28);

    //Car at 28
    addEdge(graph, 28, 34);

    addEdge(graph, 28, 29);

    //Car at 34
    addEdge(graph, 34, 4);

    //Car at 4
    addEdge(graph, 4, 10);

    //Car at 29
    addEdge(graph, 29, 30);

    //Car at 30
    addEdge(graph, 30, 36);

    //Car at 36 (end)
    addEdge(graph, 36, 37);

    addEdge(graph, 36, 35);

    addEdge(graph, 36, 6);

    //Car at 6
    addEdge(graph, 6, 5);

    //Car at 5
    addEdge(graph, 5, 11);

    //Car at 11
    addEdge(graph, 11, 12);

    shortestPath(graph, 1, 36);

    //Car at 37(END)
    drawMap(graph);

    //free(mazeHTML);

    printf("Map done\n");
}