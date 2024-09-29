#include "IncendenceMatrixGraph.h"
#include "Probability.h"

string enter_filename()
{
  string filename;
  cout << "Enter the file name: ";
  cin >> filename;
  cout << endl;
  return filename;
}


// -----------For "Graph"-----------


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


// -----------For "MatrixGraph"-----------


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


// -----------For "IncendenceMatrixGraph"-----------


int count_vertices_edges(string fileName, int &edges)
{
  ifstream fp(fileName);
  if (!fp.is_open())
  {
    cout << "Cannot open the file." << endl;
    return -1;
  }

  string line;
  int vertices = 0;
  edges = 0;
  getline(fp, line);
  vertices++;
  for (char ch : line)
    if (isdigit(ch))
      edges++;

  while (getline(fp, line))
    vertices++;

  fp.close();

  return vertices;
}