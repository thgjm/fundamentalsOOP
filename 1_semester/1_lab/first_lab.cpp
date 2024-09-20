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
};

Graph::Graph(int V) : V(V), adj(V) {};

int main()
{
  int V;
  Graph graph(V);
  return 0;
}