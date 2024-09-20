#include <iostream>
#include <vector>
#include <fstream>
using namespace std;

class Graph
{
  int V;
  vector<vector<int>> adj;

  public:
  Graph(int V);
  void read_graph_from_file();
  void print_graph();
};

Graph::Graph(int V) : V(V), adj(V) {};

void Graph::read_graph_from_file()
{
  int adj_vertex = 0, vertex = 0;
  ifstream fp("graph_test.txt");
  if (!fp.is_open())
    return;
  string line;
  while (getline(fp, line) && vertex<V)
  {
    if (line.empty())
      continue;
    
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


int main()
{
  int V=6;
  Graph graph(V);
  graph.read_graph_from_file();
  return 0;
}