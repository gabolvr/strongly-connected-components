#ifndef DATA2GRAPH_HPP
#define DATA2GRAPH_HPP

#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

using namespace std;

class data2graph {
  int n_vertices;
  vector<vector<int> > edges;
  vector<string> complexId;

 public:
  int n_edges();
  int get_n();
  vector<vector<int> > get_edges();
  vector<string>* get_nodes();
  data2graph(string filename);
};

#endif  // DATA2GRAPH_HPP
