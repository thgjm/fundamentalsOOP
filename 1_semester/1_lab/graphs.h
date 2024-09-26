#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <string>
#include <sstream>
#include <stack>

using namespace std;

//----------------------Classes----------------------

// -----------Graphs, represented with adjacency list (with vertices)-----------

class Graph
{
private:
  int V;
  vector<vector<int>> adj;
  string fileName;
  void add_the_edge_internal(int v, int w, bool directed);
  void remove_the_edge_internal(int v, int w, bool directed);
  void remove_the_vertex_internal(int v);
  void DFS(int v, bool visited[]);

protected:
  void check_the_vertex(int &vertex);
  void is_graph_directed(bool &directed);
  void printSpanningTree(int start, vector<int> &tree);

public:
  Graph(int V, string fileName) : V(V), adj(V), fileName(fileName) {};

  int get_vertices()
  {
    return V;
  }

  void read_graph_from_file();
  void print_graph();

  // these four methods involve interaction with the user
  void add_the_edge();
  void remove_the_edge();
  void add_the_vertex();
  void remove_the_vertex();

  bool isConnected();

  string graph_to_string();

  Graph complementGraph();

  int shortest_path(int start, int end);

  void findSpanningTree(int start);
};

// -----------Graphs, represented with adjacency matrix (with vertices)-----------

class MatrixGraph : Graph
{
private:
  int Vertices;
  bool **adjMatrix;
  string FileName;

  void addEdge_internal(int v, int w, bool directed);
  void removeEdge_internal(int v, int w, bool directed);
  void removeVertex_internal(int v);
  void DFS(int active, bool visited[]);

public:
  MatrixGraph(int Vertices, string filename) : Graph(Vertices, filename), Vertices(Vertices), FileName(filename)
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

  // these four methods involve interaction with the user
  void addEdge_matrix();
  void removeEdge_matrix();
  void addVertex_matrix();
  void removeVertex_matrix();

  bool isConnected_matrix();

  int shortestPath_matrix(int start, int end);

  MatrixGraph complementMatrixGraph();

  void findSpanningTree_matrix(int start);

  bool **getAdjMatrix()
  {
    return adjMatrix;
  }

  ~MatrixGraph()
  {
    for (int i = 0; i < Vertices; i++)
      delete[] adjMatrix[i];
    delete[] adjMatrix;
  }
};

//----------------------Functions----------------------

string enter_filename()
{
  string filename;
  cout << "Enter the file name: ";
  cin >> filename;
  cout << endl;
  return filename;
}

// -----------Functions for "Graph"-----------

void Graph::read_graph_from_file()
{
  int adj_vertex = 0, vertex = 0;
  ifstream fp(fileName);
  if (!fp.is_open())
  {
    cout << "Cannot open the file.";
    return;
  }

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

void Graph::remove_the_vertex_internal(int v)
{
  for (int i = 0; i < V; i++)
  {
    auto it = find(adj[i].begin(), adj[i].end(), v);
    if (it != adj[i].end())
      adj[i].erase(it);
  }

  adj.erase(adj.begin() + v);

  for (int i = 0; i < V - 1; i++)
    for (auto w = adj[i].begin(); w != adj[i].end(); w++)
      if (*w > v)
        (*w)--;

  V--;
}

void Graph::remove_the_vertex()
{
  int vertex;
  cout << "Enter the vertex you want to remove: ";

  check_the_vertex(vertex);

  remove_the_vertex_internal(vertex);
}

Graph Graph::complementGraph()
{
  Graph complementGraph(V, fileName);
  for (int v = 0; v < V; v++)
  {
    int vertices_array[V] = {0};
    for (auto i = adj[v].begin(); i != adj[v].end(); i++)
      vertices_array[*i] = 1;
    for (int j = 0; j < V; j++)
      if (v != j && vertices_array[j] == 0)
        complementGraph.add_the_edge_internal(v, j, true);
  }
  return complementGraph;
}

void Graph::DFS(int v, bool visited[])
{
  visited[v] = true;

  for (auto i = adj[v].begin(); i != adj[v].end(); i++)
    if (!visited[*i])
      DFS(*i, visited);
}

bool Graph::isConnected()
{
  bool *visited = new bool[V];
  for (int v = 0; v < V; v++)
  {
    for (int i = 0; i < V; i++)
      visited[i] = false;
    DFS(v, visited);
    for (int i = 0; i < V; i++)
      if (!visited[i])
        return false;
  }
  delete[] visited;
  return true;
}

string Graph::graph_to_string()
{
  std::ostringstream oss;
  for (int v = 0; v < V; v++)
  {
    oss << "vertex " << v << ": ";
    for (auto i = 0; i < adj[v].size(); i++)
    {
      oss << adj[v][i];
      if (i < adj[v].size() - 1)
        oss << " ";
    }
    oss << "\n";
  }
  return oss.str();
}

int Graph::shortest_path(int start, int end)
{
  if (start < 0 || start >= V || end < 0 || end >= V)
  {
    cout << "Incorrect input." << endl;
    return -1;
  }

  if (start == end)
    return 0;

  vector<bool> visited(V, false);
  vector<int> distance(V, -1);

  vector<int> queue;
  visited[start] = true;
  distance[start] = 0;
  queue.push_back(start);

  while (!queue.empty())
  {
    int current = queue.front();
    queue.erase(queue.begin());

    for (int i : adj[current])
    {
      if (!visited[i])
      {
        visited[i] = true;
        distance[i] = distance[current] + 1;

        if (i == end)
          return distance[i];

        queue.push_back(i);
      }
    }
  }

  return -1;
}

void Graph::findSpanningTree(int start)
{
  vector<bool> visited(V, false);
  vector<int> tree(V, -1);
  stack<int> st;

  st.push(start);
  visited[start] = true;

  while (!st.empty())
  {
    int u = st.top();
    st.pop();

    for (int v : adj[u])
    {
      if (!visited[v])
      {
        visited[v] = true;
        tree[v] = u;
        st.push(v);
      }
    }
  }

  printSpanningTree(start, tree);
}

void Graph::printSpanningTree(int start, vector<int> &tree)
{
  cout << "Spanning Tree:\n";
  for (int i = 0; i < V; i++)
    if (tree[i] != -1)
      cout << tree[i] << " - " << i << endl;
}


int count_vertices(string filename)
{
  int vertex = 0, vertices = -1, count = 0;
  ifstream fp(filename);
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

// -----------Functions for "MatrixGraph"-----------

MatrixGraph MatrixGraph::complementMatrixGraph()
{
  MatrixGraph complementGraph(Vertices, FileName);
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

void MatrixGraph::removeVertex_internal(int v)
{
  while (v < Vertices)
  {
    for (int i = 0; i < Vertices; ++i)
      adjMatrix[i][v] = adjMatrix[i][v];

    for (int i = 0; i < Vertices; ++i)
      adjMatrix[v][i] = adjMatrix[v][i];
    v++;
  }
  Vertices--;
}

void MatrixGraph::addVertex_matrix()
{
  int newVerticesCount = Vertices + 1;

  bool **newAdjMatrix = new bool *[newVerticesCount];
  for (int i = 0; i < newVerticesCount; ++i)
    newAdjMatrix[i] = new bool[newVerticesCount];

  for (int i = 0; i < Vertices; ++i)
    for (int j = 0; j < Vertices; ++j)
      newAdjMatrix[i][j] = adjMatrix[i][j];

  for (int i = 0; i < newVerticesCount; ++i)
  {
    newAdjMatrix[Vertices][i] = false;
    newAdjMatrix[i][Vertices] = false;
  }

  for (int i = 0; i < Vertices; ++i)
    delete[] adjMatrix[i];
  delete[] adjMatrix;

  adjMatrix = newAdjMatrix;

  Vertices++;

  cout << "Choose what vertex you want to add:\ntype 1 if isolated vertex;\n2 - if connected to the other one;\n3 - if in between others.\n";
  int answer, vertex1, current_vertex = Vertices - 1;
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

      addEdge_internal(vertex1, current_vertex, directed);
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

      removeEdge_internal(vertex1, vertex2, directed);

      addEdge_internal(vertex1, current_vertex, directed);

      addEdge_internal(current_vertex, vertex2, directed);

      break;
    }
    else
      cout << "Incorrect input. Please try again: ";
  }
}

void MatrixGraph::removeVertex_matrix()
{
  int vertex1;

  cout << "Enter the vertex you want to remove: ";

  check_the_vertex(vertex1);

  removeVertex_internal(vertex1);
}

void MatrixGraph::DFS(int active, bool visited[])
{
  visited[active] = true;
  for (int i = 0; i < Vertices; i++)
    if (adjMatrix[active][i] == 1 && !visited[i])
      DFS(i, visited);
}

bool MatrixGraph::isConnected_matrix()
{
  bool *visited = new bool[Vertices];
  for (int v = 0; v < Vertices; v++)
  {
    for (int i = 0; i < Vertices; i++)
      visited[i] = false;
    DFS(v, visited);
    for (int i = 0; i < Vertices; i++)
      if (!visited[i])
        return false;
  }
  return true;
}

int MatrixGraph::shortestPath_matrix(int start, int end)
{
  if (start < 0 || start >= Vertices || end < 0 || end >= Vertices)
  {
    cout << "Incorrect input" << endl;
    return -1;
  }

  if (start == end)
    return 0;

  vector<bool> visited(Vertices, false);
  vector<int> distance(Vertices, -1);

  vector<int> queue;
  visited[start] = true;
  distance[start] = 0;
  queue.push_back(start);

  while (!queue.empty())
  {
    int current = queue.front();
    queue.erase(queue.begin());
    for (int i = 0; i < Vertices; i++)
    {
      if (adjMatrix[current][i] && !visited[i])
      {
        visited[i] = true;
        distance[i] = distance[current] + 1;

        if (i == end)
          return distance[i];
        queue.push_back(i);
      }
    }
  }
  return -1;
}

void MatrixGraph::findSpanningTree_matrix(int start)
{
  vector<bool> visited(Vertices, false);
  vector<int> tree(Vertices, -1);
  stack<int> st;

  st.push(start);
  visited[start] = true;

  while (!st.empty())
  {
    int u = st.top();
    st.pop();

    for (int v=0; v<Vertices; v++)
    {
      if (adjMatrix[u][v] && !visited[v])
      {
        visited[v] = true;
        tree[v] = u;
        st.push(v);
      }
    }
  }

  printSpanningTree(start, tree);
}

int count_matrix_vertices(string fileName) // since the adjacency matrix should be square - it is enough to count the amount of elements in the first row
{
  ifstream fp(fileName);
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