#include <stdio.h>
//#include "pico/stdlib.h"
#include <stdlib.h>
#include <string.h>

#include "../wheelEncoder/wheelEncoder.h"
#include "../irline/irline.h"

#define SIZE 40

struct node {
  int vertex;
  struct node* next;
};

struct Graph {
  int numVertices;
  int* visited;

  // We need int** to store a two dimensional array.
  // Similary, we need struct node** to store an array of Linked lists
  struct node** adjLists;
};

struct queue {
  int items[SIZE];
  int front;
  int rear;
};

void initGraph();

extern char *mazeHTML1;
extern char *mazeHTML2;
extern char *mazeHTML3;
extern char *mazeHTML4;