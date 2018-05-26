#include <fstream>
#include <iostream>
#include <list>
#include <random>
#include <string>
#include "graph2out.hpp"

using namespace std;

const string EXTENSION =
    ".rgraph";  // extension for random graph, used by sanitizer

// create a erdos-renyi graph with n_vertices and probability for an edge = p
list<int>* ER_graph(double p, int n_vertices) {
  list<int>* edges = new list<int>[n_vertices];
  double r;
  for (unsigned i = 0; i < n_vertices; ++i) {
    for (unsigned j = 0; j < n_vertices; ++j) {
      r = double(rand()) / RAND_MAX;
      if (r < p && i != j) edges[i].push_front(j);
    }
  }
  return edges;
}

int main(int argc, char const* argv[]) {
  // picking parameters
  cout << "Hello, this module generates erdos-renyi graphs!\nPlease specify "
          "(n_vertices, prob_of_an_edge)\n";
  int n_vertices;
  double p;
  cin >> n_vertices >> p;
  cin.ignore();  // takes the trailing \n

  srand(time(0));  // really random
  list<int>* edges = ER_graph(p, n_vertices);

  // user chooses where to output the graph, true says to print the message
  graph2out::output_graph(edges, n_vertices, true);

  return 0;
}
