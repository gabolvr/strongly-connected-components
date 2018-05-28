#ifndef GRAPH2OUT_HPP
#define GRAPH2OUT_HPP

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

class graph2out {
 public:
  ostream* out_text;
  static const string EXTENSION;
  graph2out(ostream& outs = cout);
  graph2out(string out_base_name);
  ~graph2out();
  void write_graph(vector<vector<int> > edges, int n_vertices);
  static void output_graph(vector<vector<int> >, int,
                           bool should_print = false);
  static void ID2node(vector<string>, string);
};

#endif  // GRAPH2OUT_HPP
