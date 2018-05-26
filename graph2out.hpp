#ifndef GRAPH2OUT_H
#define GRAPH2OUT_H

#include <fstream>
#include <iostream>
#include <list>
#include <string>

using namespace std;

class graph2out {
 public:
  ostream* out_text;
  graph2out(ostream& outs = cout);
  graph2out(string out_base_name);
  ~graph2out();
  void write_graph(list<int>* edges, int n_vertices);
  static void output_graph(list<int>*, int, bool should_print = false);
};

#endif  // GRAPH2OUT_H
