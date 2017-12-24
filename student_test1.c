#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graph.h"
#include "assert.h"
int main() {
  Graph graph;
  char *all_edges[]= {"koala", NULL};

  init_graph(&graph);

  /* add some vertices */
  add_vertex(&graph, "hedgehog");
  add_vertex(&graph, "koala");
  add_vertex(&graph, "parrot");
  add_vertex(&graph, "kangaroo");
  add_vertex(&graph, "elephant");
  add_vertex(&graph, "naked mole rat");

  add_edge(&graph, "hedgehog", "koala", 2);

  /* uses helper function to ensure that the list of names returned by
     get_vertices() equals the expected list declared above */
  assert(compare_name_lists(get_neighbors(graph, "hedgehog"), all_edges));

  printf("No assertion remains unsatisfied!\n");

  return 0;
}
