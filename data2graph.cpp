#include "data2graph.hpp"
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

using namespace std;

/*
  This module will read snap data to create a graph
*/

int data2graph::get_n() { return n_vertices; }

vector<vector<int> > data2graph::get_edges() { return this->edges; }

vector<string>* data2graph::get_nodes() {
  return new vector<string>(this->complexId);
}

int data2graph::n_edges() {
  int counter = 0;
  for (std::vector<std::vector<int> >::iterator i = edges.begin();
       i != edges.end(); ++i) {
    counter += i->size();
  }
  return counter;
}

data2graph::data2graph(string filename) {
  ifstream raw_data;
  raw_data.open(filename);
  if (!raw_data.is_open()) {
    cerr << "Problem opening file";
    return;
  }
  string oriID, destID;
  int counter = 0;
  map<string, int> simpleId;

  while (raw_data >> oriID >> destID) {
    if (simpleId.find(oriID) == simpleId.end()) {
      simpleId[oriID] = counter;
      complexId.push_back(oriID);
      edges.push_back(vector<int>());
      counter++;
    }
    if (simpleId.find(destID) == simpleId.end()) {
      simpleId[destID] = counter;
      complexId.push_back(destID);
      edges.push_back(vector<int>());
      counter++;
    }
    edges[simpleId[oriID]].push_back(simpleId[destID]);
  }

  this->n_vertices = counter;

  raw_data.close();
}
