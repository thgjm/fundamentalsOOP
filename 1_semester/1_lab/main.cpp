#include "IncendenceMatrixGraph.h"

// -----------Main function(will be changed further)-----------

int main()
{
  // string graph_file = enter_filename();
  /*int V = count_vertices("textfiles/graph_test.txt");
  Graph graph(V, "textfiles/graph_test.txt");
  graph.read_graph_from_file();
  cout << "og graph:\n";
  graph.print_graph();
  graph.findSpanningTree(0);*/
  // string matrix_file=enter_filename();
  /*int Vertices = count_matrix_vertices("textfiles/matrix_graph_test.txt");
  MatrixGraph matrixGraph(Vertices, "textfiles/matrix_graph_test.txt");
  matrixGraph.read_matrix_from_file();
  cout << "og graph:\n";
  matrixGraph.print_list();
  matrixGraph.findSpanningTree_matrix(5);*/
  string inc_file="textfiles/incendence_matrix_graph_test.txt";
  int edges;
  int vertexes=count_vertices_edges(inc_file, edges);
  IncendenceMatrixGraph incMatrGraph(vertexes, edges, inc_file);
  incMatrGraph.readFromFile();
  cout<<"og graph:\n";
  incMatrGraph.printIncMatrix();
  cout<<endl;
  incMatrGraph.findSpanningTree_matrix(0);
  return 0;
}