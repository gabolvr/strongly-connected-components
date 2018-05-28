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
  graph2out(ostream& outs = cout);
  graph2out(string out_base_name);
  ~graph2out();
  void write_graph(vector<vector<int> > edges, int n_vertices);
  static void output_graph(vector<vector<int> >, int,
                           bool should_print = false);
};

#endif  // GRAPH2OUT_HPP
