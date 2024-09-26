#include "graphs.h"

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

// -----------Testing-----------

TEST_CASE("Testing functions for Graph class.")
{
  string filename = "graph_test.txt";
  int V = count_vertices(filename);
  Graph graph(V, filename);
  graph.read_graph_from_file();
  SUBCASE("Is graph connected?")
  {
    CHECK(graph.isConnected() == true);
  }
  SUBCASE("Check complemented graph.")
  {
    Graph complementGraph = graph.complementGraph();

    string result = complementGraph.graph_to_string();

    string expectedOutput =
        "vertex 0: 4 5\n"
        "vertex 1: 3 4 5\n"
        "vertex 2: 3 5\n"
        "vertex 3: 1 2 4 5\n"
        "vertex 4: 0 1 3\n"
        "vertex 5: 0 1 2 3\n";

    CHECK(result == expectedOutput);
  }
}

TEST_CASE("Testing functions for MatrixGraph class.")
{
  string filename = "matrix_graph_test.txt";
  int V = count_matrix_vertices(filename);
  MatrixGraph graph(V, filename);
  graph.read_matrix_from_file();
  SUBCASE("Is graph connected?")
  {
    CHECK(graph.isConnected_matrix() == true);
  }
  SUBCASE("Check complemented graph.")
  {
    MatrixGraph complementGraph = graph.complementMatrixGraph();

    bool expectedMatrix[6][6] =
        {
            {0, 1, 1, 1, 0, 0},
            {1, 0, 1, 0, 0, 0},
            {1, 1, 0, 0, 1, 0},
            {1, 0, 0, 0, 0, 0},
            {0, 0, 1, 0, 0, 1},
            {0, 0, 0, 0, 1, 0}};
    bool **adjComplementMatr = complementGraph.getAdjMatrix();
    bool **adjMatr = graph.getAdjMatrix();
    for (int i = 0; i < V; ++i)
      for (int j = 0; j < V; ++j)
        if (i != j)
        {
          if (adjMatr[i][j] == false)
            CHECK(adjComplementMatr[i][j] == true);
          else
            CHECK(adjComplementMatr[i][j] == false);
        }
  }
}