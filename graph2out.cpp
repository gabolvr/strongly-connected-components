#include "graph2out.hpp"
#include <fstream>
#include <iostream>
#include <list>
#include <string>

using namespace std;

const std::string EXTENSION = ".rgraph";
// general function that picks a graph and writes it, in the format
// "format_graph.txt", in the stream
void graph2out::write_graph(list<int>* edges, int n_vertices) {
  (*out_text) << n_vertices << endl;
  for (unsigned vert = 0; vert < n_vertices; ++vert) {
    for (list<int>::iterator dest = edges[vert].begin();
         dest != edges[vert].end(); ++dest) {
      (*out_text) << (*dest) << " ";  // problem, last one has a space
    }
    (*out_text) << endl;
  }
  return;
}

// if you want to output the graph to a file use this
graph2out::graph2out(string out_base_name) {
  ofstream* fout = new ofstream(out_base_name + EXTENSION);
  out_text = fout;
}

graph2out::graph2out(ostream& outs) { out_text = &outs; }

graph2out::~graph2out() {
  if (out_text != &cout) {
    ofstream* derfrncr = dynamic_cast<ofstream*>(out_text);
    derfrncr->close();
    delete derfrncr;
  }
}

void graph2out::output_graph(list<int>* edges, int n_vertices,
                             bool should_print) {
  if (should_print)
    cout
        << "\nIf you want to output the graph in a file type the name you want "
           "(no extension).\nOtherwise press Enter\n";
  string out_base_name;
  getline(cin, out_base_name);  // attention! buffer must be clear
  graph2out* gen_g;
  if (out_base_name == "") {
    gen_g = new graph2out(cout);
  } else {
    gen_g = new graph2out(out_base_name);
  }
  gen_g->write_graph(edges, n_vertices);
  delete gen_g;
}
