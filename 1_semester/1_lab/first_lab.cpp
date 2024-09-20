#include <iostream>
#include <vector>
#include <fstream>
using namespace std;

class Graph
{
private:
  int V;
  vector<vector<int>> adj;
  void add_the_edge_internal(int v, int w, bool directed);

public:
  Graph(int V);
  void read_graph_from_file();
  void print_graph();
  void add_the_edge();
};

Graph::Graph(int V) : V(V), adj(V) {};

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
  while (check)
  {
    cin >> vertex1;
    if(vertex1 <0 || vertex1 > V-1) 
      cout << "Incorrect input of the first vertex. Please try again: ";
    else check=false;
  }
  check=true;

    while (check)
  {
    cin >> vertex2;
    if(vertex2 <0 || vertex2 > V-1) 
      cout << "Incorrect input of the second vertex. Please try again: ";
    else check=false;
  }

  check=true;

  cout << "Specify if the graph is directed or not (type 'y' for yes, 'n' for no): ";
  char answer;
  while (check)
  {
    cin >> answer;
    switch (answer)
    {
    case 'y':
      directed = true;
      check = false;
      break;
    case 'n':
      directed = false;
      check = false;
      break;
    default:
      cout << "Incorrect input. Please try again: ";
      break;
    }
  }
  add_the_edge_internal(vertex1, vertex2, directed);
}

int main()
{
  int V = 6;
  Graph graph(V);
  graph.read_graph_from_file();
  cout << "old graph:\n";
  graph.print_graph();
  graph.add_the_edge();
  cout << "\n\nnew graph:\n";
  graph.print_graph();
  return 0;
}