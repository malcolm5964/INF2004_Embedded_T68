#include <stdio.h>
//#include "pico/stdlib.h"
#include <stdlib.h>

#include "../wheelEncoder/wheelEncoder.h"
#include "../irline/irline.h"

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

void initGraph();