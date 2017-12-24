#if !defined(GRAPH_DATASTRUCTURE_H)
#define GRAPH_DATASTRUCTURE_H

typedef struct edge {
  char *source;
  char *dest;
  int cost;
  struct edge *right;
  struct edge *left;
} Edge;

typedef struct vertex {
  char *name;
  struct vertex *right;
  struct vertex *left;
  int num_edges;
  Edge *root_edge;
} Vertex;

typedef struct graph {
  Vertex *root_vertex;
  int num_vertices;
} Graph;

#endif
  
