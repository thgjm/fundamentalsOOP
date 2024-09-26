#include "graphs.h"

// -----------Main function(will be changed further)-----------

int main()
{
  // string graph_file = enter_filename();
  /*int V = count_vertices("graph_test.txt");
  Graph graph(V, "graph_test.txt");
  graph.read_graph_from_file();
  cout << "og graph:\n";
  graph.print_graph();
  graph.findSpanningTree(0);*/
  // string matrix_file=enter_filename();
  int Vertices = count_matrix_vertices("matrix_graph_test.txt");
  MatrixGraph matrixGraph(Vertices, "matrix_graph_test.txt");
  matrixGraph.read_matrix_from_file();
  cout << "og graph:\n";
  matrixGraph.print_list();
  matrixGraph.findSpanningTree_matrix(5);
  return 0;
}