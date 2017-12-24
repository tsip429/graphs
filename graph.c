/* Tsipora Stone, 114110213, 0305, tstone97 */
/* takes in a graph and represents it in two binary search trees - one for
   edges and one for vertices and manipulates the graph by adding vertices and 
   adding edges and returning edges and other functions*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graph.h"

/*all helper method declarations */
static Vertex *get_v(Vertex *vertex, const char name[]);
static void add_v_help(Vertex *vertex, const char name[], Vertex *added_vertex);
static void get_vertices_help(Vertex *vertex, char **vertices, int *pos);
static int add_e_help(Edge *edge, const char dest[], Edge *added_edge);
static Edge *get_e(Edge *edge, const char dest[]);
static void get_edges_help(Edge *edge, char **edges, int *pos);

/* initializes graph by initializing the root vertex and the num_vertices in 
   the graph structure */
void init_graph(Graph *graph) {
  graph -> root_vertex = NULL;
  graph -> num_vertices = 0;
}

/* helper method to return the vertex that matches an already existent vertex in
   the graph */
static Vertex *get_v(Vertex *vertex, const char name[]) {
  /* base case - if vertex is NULL it didn't find a match to name and if theyre
     equal returns that vertex */
  if (vertex == NULL || strcmp(vertex -> name, name) == 0)
    return vertex;
  else {
    /* recurses left or right depending of the name is bigger than the vertex 
       name or smaller */
    if (strcmp(vertex -> name, name) < 0)
      return get_v(vertex -> right, name);
    if (strcmp(vertex -> name, name) > 0)
      return get_v(vertex -> left, name);
  }
  return NULL;
}

/* helper method to add a new vertex to the graph that has name */
static void add_v_help(Vertex *vertex, const char name[], Vertex *added_vertex) {
  if (vertex != NULL) {
    /* if the name is greater than the vertex name recurses to the right */
    if (strcmp(vertex -> name, name) < 0) {
      if (vertex -> right == NULL) {
	vertex -> right = added_vertex;
      }
      /* when vertex -> right is NULL this is where the vertex should be added
	 in the tree */
      else
	add_v_help(vertex -> right, name, added_vertex);
    }
    /* if the name is less than vertex name recurses to left */
    else {
      if (vertex -> left == NULL) {
	vertex -> left = added_vertex;
      }
      /* when vertex -> left is NULL this is where the vertex should be added 
	 in the tree */
      else
	add_v_help(vertex -> left, name, added_vertex);
    }
  }
}

/* this function calls the helper method to add the vertex and also mallocs the
   new vertex to be added to the correct size */
int add_vertex(Graph *graph, const char new_vertex[]) {
  int rval = 0;
  Vertex *new_v;
  if (graph != NULL && new_vertex != NULL) {
    new_v = malloc(sizeof(Vertex));
    /* mallocs the name of the new vertex to be the right size for new_vertex */
    new_v -> name = malloc(sizeof(char) * (strlen(new_vertex) + 1));
    strcpy(new_v -> name, new_vertex);
    /* sets everything to NULL */
    new_v -> root_edge = NULL;
    new_v -> left = NULL;
    new_v -> right = NULL;
    /* if at the beginning of the tree add to the root vertex */
    if (graph -> root_vertex == NULL) {
      graph -> root_vertex = new_v;
      rval = 1;
    }
    /* if not at the beginning call helper funtion and add to the tree */
    else {
      if (get_v(graph -> root_vertex, new_vertex) == 0) {
	add_v_help(graph -> root_vertex, new_vertex, new_v);
	rval = 1;
      }
    }
  }
  /* if the vertex was successfully added increment num_vertices */
  if (rval == 1)
    graph -> num_vertices++;
  return rval;  
}

/* returns the number of vertices in the graph */
int num_vertices(Graph graph) {
  return graph.num_vertices;
}

/* checks if the graph has the vertex with name "name" */
int has_vertex(Graph graph, const char name[]) {
  int rval = 0;
  if (get_v(graph.root_vertex, name))
    rval = 1;
  return rval;
}

/* helper method for returning vertices and storing in an array of pointers 
   to names does an inorder traversal */
static void get_vertices_help(Vertex *vertex, char **vertices, int *pos) {
  if (vertex != NULL) {
    /* recurses over the left subtree until it's NULL */
    if (vertex -> left != NULL) {
      get_vertices_help(vertex -> left, vertices, pos);
    }
    /* mallocs element in the vertices array to the size of the vertex names */
    (vertices)[*pos] = malloc((strlen(vertex -> name) + 1));
    /* copies the vertex name into the vertices array at the right position */
    strcpy(vertices[*pos], vertex -> name);
    (*pos)++;
    /* recurses over the right subtree until it's NULL */
    if (vertex -> right != NULL) {
      get_vertices_help(vertex -> right, vertices, pos);
    }
  }
}

/* function that calls the helper method and returns the resulting new
   array */
char **get_vertices (Graph graph) {
  char **vertices = malloc((graph.num_vertices + 1) * sizeof(char*));
  int pos = 0;
  get_vertices_help(graph.root_vertex, vertices, &pos);
  /*sets the last elment in the vertices array to NULL */
  vertices[graph.num_vertices] = NULL;
  return vertices;
}

/* helper function to add an edge recursively to the edge tree */
static int add_e_help(Edge *edge, const char dest[], Edge *added_edge) {
  if (edge != NULL) {
    /* if the destination vertex is equal to the edge destination vertex add
       the cost of the new edge to the edge */
    if (strcmp(edge -> dest, dest) == 0) {
      edge -> cost = added_edge -> cost;
      return 0;
    }
    /* if the dest vertex of the new edge is greater lexigraphically than the 
       edge in the tree recurse to the right */
    else if (strcmp(edge -> dest, dest) < 0) {
      if (edge -> right != NULL)
	return add_e_help(edge -> right, dest, added_edge);
      /* if the right tree is NULL add to this part of the tree */
      else {
	edge -> right = added_edge;
	return 1;
      }
      return 0;
    }
    /* if the dest vertex of the new edge is smaller lexigraphically than the 
       edge in the tree recurse to the left */
    else {
      if (edge -> left != NULL) 
	return add_e_help(edge -> left, dest, added_edge);
      /* if the left tree is NULL add to this part of the tree */
      else {
	edge -> left = added_edge;
	return 1;
      }
    }
  }
  else
    edge = added_edge;
  return 0;
}

/* adds an edge to the graph and calls the helper function and initializes the
   edge and the names and mallocs everything to the correct size */
int add_edge(Graph *graph, const char source[], const char dest[], int cost) {
  int rval = 0;
  Vertex *source_vertex;
  Edge *edge = malloc(sizeof(Edge));
  edge -> source = malloc((strlen(source) + 1) * sizeof(char));
  edge -> dest = malloc((strlen(source) + 1) * sizeof(char));
  /* copies the source string to the edge source */
  strcpy(edge -> source, source);
  /* copies the dest string to the edge dest */
  strcpy(edge -> dest, dest);
  edge -> cost = cost;
  edge -> left = edge -> right = NULL;
  if (graph != NULL && source != NULL && dest != NULL && cost >= 0) {
    /* if the source vertex doesn't exist add the vertex to the graph */
    if (!get_v(graph -> root_vertex, source))
      add_vertex(graph, source);
    /* if the source vertex doesn't exist add the vertex to the graph */
    if (!get_v(graph -> root_vertex, dest))
      add_vertex(graph, dest);
    /* sets the source vertex to the value returned by the get_v function */
    source_vertex = get_v(graph -> root_vertex, source);
    /* if the root of the tree is NULL - nothing in the tree add to the root
       edge */
    if (source_vertex -> root_edge == NULL) {
      source_vertex -> root_edge = edge;
      source_vertex -> num_edges++;
      /* adds to the number of edges by 1 - the return val of add_e_help */
    } else {
      source_vertex -> num_edges +=
	add_e_help(source_vertex -> root_edge, edge -> dest, edge);
    }
    rval = 1;
  }
  return rval;   
}

/* a helper function to find and return an edge with the name dest */
static Edge *get_e(Edge *edge, const char dest[]) {
  if (edge == NULL || strcmp(edge -> dest, dest) == 0)
    return edge;
  else {
    if (strcmp(edge -> dest, dest) < 0)
      return get_e(edge -> right, dest);
    if (strcmp(edge -> dest, dest) > 0)
      return get_e(edge -> left, dest);
  }
  return NULL;
}

/* returns the cost of the edge that matches the source and dest names */
int get_edge_cost(Graph graph, const char source[], const char dest[]){
  int cost = -1;
  Vertex *vertex;
  Edge *edge;
  if(source != NULL && dest != NULL) {
    /* sets vertex to the value returned by get_v */
    vertex = get_v(graph.root_vertex, source);
    if (vertex != NULL) {
      /* sets edge to the value returned by get_e */
      edge = get_e(vertex -> root_edge, dest);
      /* if both vertex and edge exist in the graph set cost to that edge's cost
       */
      if (vertex && edge) {
        cost =  edge -> cost;
      }
    }
  }
  return cost;
}

/* returns the number of neighbors that a vertex has */
int num_neighbors(Graph graph, const char vertex[]) {
  int rval = -1;
  if (vertex != NULL) {
    /* if vertex exists in the graph set rval to the number of edges  */
    if (get_v(graph.root_vertex, vertex)) {
      rval = get_v(graph.root_vertex, vertex) -> num_edges;
    }
  }
  return rval;
}

/* helper method for returning the neighbors of a vertex and storing in an 
   array of pointers to names does an inorder traversal */
static void get_edges_help(Edge *edge, char **edges, int *pos) {
  if (edge != NULL) {
    /* calls function on the left subtree until it's NULL */
    if (edge -> left != NULL) {
      get_edges_help(edge -> left, edges, pos);
    } /* when edge -> left is NULL mallocs each element of the edges array to
	 the size of each edge name */
    (edges)[*pos] = malloc((strlen(edge -> dest) + 1));
    /* copies all the edges into the array */
    strcpy(edges[*pos], edge -> dest);
    (*pos)++;
    /* calls function on the left subtree until it's NULL */
    if (edge -> right != NULL) {
      get_edges_help(edge -> right, edges, pos);
    }
  }
}

/* returns the neighbors of the vertex with name "vertex" */
char **get_neighbors(Graph graph, const char vertex[]) {
  Vertex *new_vertex;
  char **edges;
  int pos;
  new_vertex = get_v(graph.root_vertex, vertex);
  /* if vertex is NULL or the vertex doesn't exist in the graph return NULL */
  if (vertex == NULL || new_vertex == NULL)
    return NULL;
  /* mallocs the array of pointers to edge names to the amount of edges */
   edges = malloc((new_vertex -> num_edges + 1) * sizeof(char*));
  pos = 0;
  /* calls the helper function on the root_edge and edges with pos at 0 */
  get_edges_help(new_vertex -> root_edge, edges, &pos);
  /* makes the last element in the array NULL */
  edges[new_vertex -> num_edges] = NULL;
  return edges;
}

