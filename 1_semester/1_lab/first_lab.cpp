#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
using namespace std;

// -----------Graphs, represented with adjacency list (with vertices)-----------

class Graph
{
private:
  int V;
  vector<vector<int>> adj;
  void add_the_edge_internal(int v, int w, bool directed);
  void remove_the_edge_internal(int v, int w, bool directed);

protected:
  void check_the_vertex(int &vertex);
  void is_graph_directed(bool &directed);

public:
  Graph(int V) : V(V), adj(V) {};
  int get_vertices()
  {
    return V;
  }
  void read_graph_from_file();
  void print_graph();
  void add_the_edge();
  void remove_the_edge();
  void add_the_vertex();
  void remove_the_vertex(); // not yet implemented

  Graph complementGraph();
};

void Graph::read_graph_from_file()
{
  int adj_vertex = 0, vertex = 0;
  ifstream fp("graph_test.txt");
  if (!fp.is_open())
    return;
  string line;
  while (getline(fp, line) && vertex < V)
  {

    if (line.empty())
    {
      vertex++;
      continue;
    }

    int i = 0;
    while (i < line.size())
    {
      adj_vertex = 0;
      if (line[i] == ' ')
      {
        i++;
        continue;
      }
      while (i < line.size() && isdigit(line[i]))
      {
        adj_vertex = adj_vertex * 10 + (line[i] - '0');
        i++;
      }
      adj[vertex].push_back(adj_vertex);

      i++;
    }
    vertex++;
  }
  fp.close();
}

void Graph::print_graph()
{
  for (int v = 0; v < V; v++)
  {
    cout << "vertex " << v << ": ";
    for (auto i = adj[v].begin(); i != adj[v].end(); i++)
      cout << *i << " ";
    if (adj[v].empty())
      cout << "∅";
    cout << endl;
  }
}

void Graph::check_the_vertex(int &vertex)
{
  while (true)
  {
    cin >> vertex;
    if (vertex < 0 || vertex > V - 1)
      cout << "Incorrect input of the vertex. Please try again: ";
    else
      break;
  }
}

void Graph::is_graph_directed(bool &directed)
{
  cout << "Specify if the graph is directed or not (type 'y' for yes, 'n' for no): ";
  char answer;

  while (true)
  {
    cin >> answer;
    if (answer == 'y')
    {
      directed = true;
      break;
    }
    else if (answer == 'n')
    {
      directed = false;
      break;
    }
    else
      cout << "Incorrect input. Please try again: ";
  }
}

void Graph::add_the_edge_internal(int v, int w, bool directed)
{
  adj[v].push_back(w);
  if (!directed)
    adj[w].push_back(v);
}

void Graph::add_the_edge()
{
  int vertex1, vertex2;
  bool directed, check = true;

  cout << "Enter the two already existing vertexes between which you want to add an edge: ";

  check_the_vertex(vertex1);
  check_the_vertex(vertex2);

  is_graph_directed(directed);

  add_the_edge_internal(vertex1, vertex2, directed);
}

void Graph::remove_the_edge_internal(int v, int w, bool directed)
{
  auto it = find(adj[v].begin(), adj[v].end(), w);
  if (it != adj[v].end())
    adj[v].erase(it);

  if (!directed)
  {
    it = find(adj[w].begin(), adj[w].end(), v);
    if (it != adj[w].end())
      adj[w].erase(it);
  }
}

void Graph::remove_the_edge()
{
  int vertex1, vertex2;
  bool directed;
  cout << "Enter the two already existing vertexes between which you want to remove the edge: ";

  check_the_vertex(vertex1);
  check_the_vertex(vertex2);

  is_graph_directed(directed);

  remove_the_edge_internal(vertex1, vertex2, directed);
}

void Graph::add_the_vertex()
{
  V++;
  adj.push_back(vector<int>());
  cout << "Choose what vertex you want to add:\ntype 1 if isolated vertex;\n2 - if connected to the other one;\n3 - if in between others.\n";
  int answer, vertex1, current_vertex = V - 1;
  while (true)
  {
    cin >> answer;
    if (answer == 1)
      break;
    else if (answer == 2)
    {
      bool directed;
      cout << "Type in the vertex to which this one is be connected: ";
      check_the_vertex(vertex1);

      is_graph_directed(directed);

      add_the_edge_internal(vertex1, current_vertex, directed);
      break;
    }
    else if (answer == 3)
    {
      int vertex2;
      bool directed;
      cout << "Type in the vertexes between which this one is connected: ";

      check_the_vertex(vertex1);

      check_the_vertex(vertex2);

      is_graph_directed(directed);

      remove_the_edge_internal(vertex1, vertex2, directed);

      add_the_edge_internal(vertex1, current_vertex, directed);

      add_the_edge_internal(current_vertex, vertex2, directed);

      break;
    }
    else
      cout << "Incorrect input. Please try again: ";
  }
}

Graph Graph::complementGraph()
{
  Graph complementGraph(V);
  for (int v = 0; v < V; v++)
  {
    int vertices_array[V] = {0};
    for (auto i = adj[v].begin(); i != adj[v].end(); i++)
      vertices_array[*i] = 1;
    for (int j = 0; j < V; j++)
      if (v!=j && vertices_array[j] == 0)
        complementGraph.add_the_edge_internal(v, j, true);
  }
  return complementGraph;
}

int count_vertices()
{
  int vertex = 0, vertices = -1, count = 0;
  ifstream fp("graph_test.txt");
  if (!fp.is_open())
    return -1;
  string line;
  while (getline(fp, line))
  {
    if (line.empty())
    {
      vertex++;
      vertices = max(vertices, vertex);
      continue;
    }

    int i = 0;
    while (i < line.size())
    {
      if (line[i] == ' ')
      {
        i++;
        continue;
      }
      count = 0;
      while (i < line.size() && isdigit(line[i]))
      {
        count = count * 10 + (line[i] - '0');
        i++;
      }
      i++;
      vertices = max(vertices, count + 1);
    }
    vertex++;
    vertices = max(vertices, vertex);
  }
  fp.close();
  return vertices;
}

// -----------Graphs, represented with adjacency matrix (with vertices)-----------

class MatrixGraph : Graph
{
private:
  int Vertices;
  bool **adjMatrix;
  void addEdge_internal(int v, int w, bool directed);
  void removeEdge_internal(int v, int w, bool directed);

public:
  MatrixGraph(int Vertices) : Graph(Vertices), Vertices(Vertices)
  {
    adjMatrix = new bool *[Vertices];
    for (int i = 0; i < Vertices; i++)
    {
      adjMatrix[i] = new bool[Vertices];
      for (int j = 0; j < Vertices; j++)
        adjMatrix[i][j] = false;
    }
  }
  int getVertices()
  {
    return Vertices;
  }
  void read_matrix_from_file();
  void print_matrix();
  void print_list();
  void addEdge_matrix();
  void removeEdge_matrix();

  MatrixGraph complementMatrixGraph();

  ~MatrixGraph()
  {
    for (int i = 0; i < Vertices; i++)
      delete[] adjMatrix[i];
    delete[] adjMatrix;
  }
};

MatrixGraph MatrixGraph::complementMatrixGraph()
{
  MatrixGraph complementGraph(Vertices);
  for (int i = 0; i < Vertices; i++)
    for (int j = 0; j < Vertices; j++)
    {
      if (i != j && !adjMatrix[i][j])
        complementGraph.addEdge_internal(i, j, true);
    }
  return complementGraph;
}

void MatrixGraph::read_matrix_from_file()
{
  ifstream fp("matrix_graph_test.txt");
  if (!fp.is_open())
  {
    cout << "Cannot open the file." << endl;
    return;
  }

  string line;
  int vertex = 0;

  while (getline(fp, line) && vertex < Vertices)
  {
    int adj_vertex = 0;
    for (char ch : line)
      if (ch == '1' || ch == '0')
      {
        if (ch == '1')
          adjMatrix[vertex][adj_vertex] = true;
        adj_vertex++;
      }
    vertex++;
  }

  fp.close();
}

void MatrixGraph::print_matrix()
{
  for (int i = 0; i < Vertices; i++)
  {
    for (int j = 0; j < Vertices; j++)
      cout << adjMatrix[i][j] << " ";
    cout << endl;
  }
}

void MatrixGraph::print_list()
{
  for (int i = 0; i < Vertices; i++)
  {
    cout << "vertex " << i << ": ";
    int check = 0;
    for (int j = 0; j < Vertices; j++)
      if (adjMatrix[i][j])
      {
        cout << j << " ";
        check++;
      }
    if (check == 0)
      cout << "∅";
    cout << endl;
  }
}

void MatrixGraph::addEdge_internal(int v, int w, bool directed)
{
  adjMatrix[v][w] = true;
  if (!directed)
    adjMatrix[w][v] = true;
}

void MatrixGraph::addEdge_matrix()
{
  int vertex1, vertex2;
  bool directed, check = true;

  cout << "Enter the two already existing vertexes between which you want to add an edge: ";

  check_the_vertex(vertex1);
  check_the_vertex(vertex2);

  is_graph_directed(directed);

  addEdge_internal(vertex1, vertex2, directed);
}

void MatrixGraph::removeEdge_internal(int v, int w, bool directed)
{
  adjMatrix[v][w] = false;
  if (!directed)
    adjMatrix[w][v] = false;
}

int count_matrix_vertices() // since the adjacency matrix should be square - it is enough to count the amount of elements in the first row
{
  ifstream fp("matrix_graph_test.txt");
  if (!fp.is_open())
  {
    cout << "Cannot open the file." << endl;
    return -1;
  }

  string line;
  int vertices = 0;

  getline(fp, line);

  for (char ch : line)
    if (ch == '1' || ch == '0')
      vertices++;

  fp.close();

  return vertices;
}

// -----------Main function-----------

int main()
{
  int V = count_vertices();
  Graph graph(V);
  graph.read_graph_from_file();
  cout << "og graph:\n";
  graph.print_graph();
  Graph complementGraph = graph.complementGraph();
  cout << "complement graph:\n";
  complementGraph.print_graph();
  /*int Vertices = count_matrix_vertices();
  MatrixGraph matrixGraph(Vertices);
  matrixGraph.read_matrix_from_file();
  cout << "og graph:\n";
  matrixGraph.print_matrix();
  MatrixGraph complementGraph_matrix = matrixGraph.complementMatrixGraph();
  cout << "complement graph:\n";
  complementGraph_matrix.print_matrix();*/
  return 0;
}