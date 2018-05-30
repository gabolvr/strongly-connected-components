#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include "data2graph.hpp"
#include "graph2out.hpp"

using namespace std;

int main(int argc, char const *argv[]) {
  string finname, foutname, name = argv[1];
  cout << "Name of the data set? (input extension .dgraph , output extension "
          ".rgraph)\n";
  finname = name + ".dgraph";
  foutname = name;
  cout << "Opening file " << finname << "..." << endl
       << "and creating graph" << endl;
  data2graph d2g(finname);
  cout << "Graph successfully created" << endl;

  cout << "Opening file " << foutname << "..." << endl;
  graph2out g2o(foutname);
  cout << "Writing graph in " << foutname << "..." << endl;
  g2o.write_graph(d2g.get_edges(), d2g.get_n());
  cout << "Graph successfully written. Job concluded" << endl;
  cout << "Quick analysis:" << endl;
  cout << "Number of nodes in the graph: " << d2g.get_n() << endl;
  cout << "n_of_edges in the graph: " << d2g.n_edges() << endl;

  return 0;
}
