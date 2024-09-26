#include "graphs.h"

// -----------Main function(will be changed further)-----------

int main()
{
  // string graph_file = enter_filename();
  int V = count_vertices("graph_test.txt");
  Graph graph(V, "graph_test.txt");
  graph.read_graph_from_file();
  cout << "og graph:\n";
  graph.print_graph();
  cout<<graph.shortest_path(0, 1);
  // string matrix_file=enter_filename();
  /*int Vertices = count_matrix_vertices("matrix_graph_test.txt");
  MatrixGraph matrixGraph(Vertices, "matrix_graph_test.txt");
  matrixGraph.read_matrix_from_file();
  cout << "og graph:\n";
  matrixGraph.print_matrix();
  matrixGraph.addVertex_matrix();
  cout << "new graph:\n";
  matrixGraph.print_matrix();*/
  return 0;
}