# Block Diagram
![Embedded Diagram (1)](https://github.com/malcolm5964/INF2004_Embedded_T68/assets/25952539/462bd128-cb81-4fe6-9310-f2eafe0ffd90)
- Blue Rectangle = Hardware Compoenents
- Green Circle = Software Components

# Data Flow Diagram
![Embedded Diagram](https://github.com/malcolm5964/INF2004_Embedded_T68/assets/25952539/89ea4c7e-99b6-40d1-9032-8a09c578d9ef)
- Ultrasonic, Magnometer, Wheel Encoder and IR Sensor only needs to get data for Mapping.c and Barcode.c to perform logic and calculations.
- Mapping.c will return which direction and distance the car should move next with the Heading Degree data and the IR Sensor data to detect if there is a wall or not.
- Motor.c requires information to decide the car movement
  - Information from Mapping.c will be pass to decide which direction the wheel should turn and distance to move.
  - Information from wheelEncoder.c to decide when to stop if the distance is reach or control the speed(PID)
  - Information from Magnometer.c to decide when to stop turning.
- Barcode.c will decode the data gathered from IR Sensor to an Alphabet
- Wifi Server will request for the Map Data and decoded barcode information and display it with the help of ssi and cgi handlers.




# Flow Chart

1. Motor Controller
<p align="center">
  <img width="141" alt="image" src="https://github.com/malcolm5964/INF2004_Embedded_T68/assets/25952539/69d18d03-363d-4fbd-9aa3-d35b0db7920d">
  <p>Flow of motor controller and actions to make the car straight</p>
</p>

2. Wheel Encoder
<p align="center">
<img width="190" alt="image" src="https://github.com/malcolm5964/INF2004_Embedded_T68/assets/25952539/3d8e0ae1-ba55-450a-a786-a6098eb8c14d">
<p>Flow of Wheel Encoder and steps to calculate distance and speed</p>
</p>

2. Magnometer
<p align="center">
<img width="275" alt="image" src="https://github.com/malcolm5964/INF2004_Embedded_T68/assets/25952539/0980a073-e760-46de-9d8e-355d84518202">
<p>Flow of Magnometer and how it turn 90 degree</p>
</p>

3. Ultrasonic
<p align="center">
<img width="172" alt="Screenshot 2023-11-25 234754" src="https://github.com/malcolm5964/INF2004_Embedded_T68/assets/25952539/4eba62cc-67ce-4f8c-99e1-28b21faabd2c">
<p>Flow of ultrasonic on how it detects an obstacle</p>
</p>

# Mapping Algorithm

1. Create an Adjacency List for each Vertex to store vertices connected to it.<br>
Vertex 1:<br>
Vertex 2:<br>
Vertex 3:<br>
Vertex 4:<br>
Vertex 5:<br>
.
.
.<br>
Vertex 36:

2. Check Vertex 1
<img width="887" alt="image" src="https://github.com/malcolm5964/INF2004_Embedded_T68/assets/25952539/f66ce54b-d37c-4671-ba49-763be73d24e4">


3. Store the wall readings into the adjacency list<br>
Vertex 0: None<br>
Vertex 1: 2 -> 7 -> 31<br>
Vertex 2: 1<br>
Vertex 7: 1<br>
Vertex 31: 1

4. Go through adjacency list to find next vertex to check
Next vertex is 2.
Position car at vertex 2 and run CheckVertex().

5. Repeat for all vertices, Car will return back to start
<img width="826" alt="image" src="https://github.com/malcolm5964/INF2004_Embedded_T68/assets/25952539/d3e741af-9717-4eac-8c77-cc880b222354">

# Maze Drawing
Creating the map as HTML table with borders to be pass into the ssi handler
```
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
      snprintf(mazeHTML + strlen(mazeHTML), bufferSize - strlen(mazeHTML), "<tr>");

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

        snprintf(mazeHTML + strlen(mazeHTML), bufferSize - strlen(mazeHTML), "</tr>");
        printf("\nHTML %s\n", mazeHTML);
        number++;
    }
    else {
      continue;
    }

  }
}
```

# Finding Shortest Path
Use BFS with a prev list to track which vertex connects to which 
```
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
```
