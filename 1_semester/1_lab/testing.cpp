#include "classes/IncendenceMatrixGraph.h"

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "classes/doctest.h"

// -----------Testing-----------

TEST_CASE("Testing functions for Graph class.")
{
  string filename = "graph_test.txt";
  int V = count_vertices(filename);
  Graph graph(V, filename);
  graph.read_graph_from_file();

  SUBCASE("Is graph connected?")
  CHECK(graph.isConnected() == true);

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

  SUBCASE("Check shortest path.")
  {
    SUBCASE("Between 0 and 1.")
    CHECK(graph.shortest_path(0, 1) == 1);

    SUBCASE("Between 0 and 5.")
    CHECK(graph.shortest_path(0, 5) == 3);
  }
}

TEST_CASE("Testing functions for MatrixGraph class.")
{
  string filename = "matrix_graph_test.txt";
  int V = count_matrix_vertices(filename);
  MatrixGraph graph(V, filename);
  graph.read_matrix_from_file();

  SUBCASE("Is graph connected?")
  CHECK(graph.isConnected_matrix() == true);

  SUBCASE("Check complemented graph.")
  {
    MatrixGraph complementGraph = graph.complementMatrixGraph();

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

  SUBCASE("Check shortest path.")
  {
    SUBCASE("Between 0 and 1.")
    CHECK(graph.shortestPath_matrix(0, 1) == 1);

    SUBCASE("Between 0 and 5.")
    CHECK(graph.shortestPath_matrix(0, 5) == 3);
  }
}

TEST_CASE("Testing functions for IncendenceMatrixGraph class.")
{
  string filename = "incendence_matrix_graph_test.txt";
  int edges;
  int vertices = count_vertices_edges(filename, edges);
  IncendenceMatrixGraph graph(vertices, edges, filename);
  graph.readFromFile();

  SUBCASE("Is graph connected?")
  CHECK(graph.isConnected_incMatrix() == true);
  SUBCASE("Complemented graph.")
  {
    IncendenceMatrixGraph complementGraph = graph.complementMatrixGraph();

    int expectedMatrix[6][9] =
        {
            {1, 1, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 1, 1, 1, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 1, 1, 0, 0},
            {0, 0, 1, 0, 0, 1, 0, 1, 1},
            {1, 0, 0, 1, 0, 0, 0, 1, 0},
            {0, 1, 0, 0, 1, 0, 1, 0, 1}};

    for (int i = 0; i < complementGraph.getVertexes(); i++)
      for (int j = 0; j < complementGraph.getEdges(); j++)
        CHECK(complementGraph.getIncMatrix()[i][j] == expectedMatrix[i][j]);
  }
  SUBCASE("Check shortest path.")
  {
    SUBCASE("Between 0 and 1.")
    CHECK(graph.shortestPath_incMatrix(0, 1) == 1);

    SUBCASE("Between 3 and 5.")
    CHECK(graph.shortestPath_incMatrix(3, 5) == 4);
  }
}