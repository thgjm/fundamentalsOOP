#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
using namespace std;

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
      vertices=max(vertices, vertex);
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
  cout << vertices << "\n\n";
  return vertices;
}

class Graph
{
private:
  int V;
  vector<vector<int>> adj;
  void add_the_edge_internal(int v, int w, bool directed);
  void remove_the_edge_internal(int v, int w, bool directed);

public:
  Graph(int V);
  int get_vertices();
  void read_graph_from_file();
  void print_graph();
  void add_the_edge();
  void remove_the_edge();
  void check_the_vertex(int &vertex);
  void is_graph_directed(bool &directed);
  void add_the_vertex();
  void remove_the_vertex();
};

Graph::Graph(int V) : V(V), adj(V) {};

int Graph::get_vertices()
{
  return V;
}

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
      adj_vertex = 0;
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

int main()
{
  int V = count_vertices();
  Graph graph(V);

  graph.read_graph_from_file();
  cout << "old graph:\n";
  graph.print_graph();
  // graph.add_the_vertex();
  // cout << "\n\nnew graph:\n";
  // graph.print_graph();
  return 0;
}