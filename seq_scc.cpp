#include <fstream>
#include <iostream>
#include <list>
#include <sstream>
#include <stack>
#include <string>
#include <vector>
#include "graph2out.hpp"

using namespace std;

vector<vector<int> > read_edges(int n_vertices, istream& in) {
  vector<vector<int> > neighbors(n_vertices, vector<int>());

  int dest;
  string line;
  for (unsigned i = 0; i < n_vertices; ++i) {
    getline(in, line);
    stringstream estream(line);
    while (estream >> dest) {
      neighbors[i].push_back(dest);
    }
  }
  return neighbors;
}

vector<vector<int> > rev_edges(int n_vertices,
                               vector<vector<int> >& neighbors) {
  vector<vector<int> > neighbors_rev(n_vertices, vector<int>());
  for (unsigned i = 0; i < n_vertices; ++i) {
    for (vector<int>::const_iterator j = neighbors[i].begin();
         j != neighbors[i].end(); ++j) {
      neighbors_rev[*j].push_back(i);
    }
  }
  return neighbors_rev;
}

void printSCC(list<list<int> >& scc) {
  for (std::list<list<int> >::iterator i = scc.begin(); i != scc.end(); ++i) {
    for (std::list<int>::iterator j = i->begin(); j != i->end(); ++j)
      cout << *j << " ";
    cout << endl;
  }
}

// this returns the list of nodes reachable by dfs in the opposite order of
// ending
list<int> dfs(int current, vector<bool>& visited,
              vector<vector<int> >& neighbors) {
  list<int> reached;
  visited[current] = true;

  for (vector<int>::iterator i = neighbors[current].begin();
       i != neighbors[current].end(); ++i) {
    if (!visited[*i])
      reached.splice(reached.end(), dfs(*i, visited, neighbors));
  }

  reached.push_back(current);
  return reached;
}

// this will return the components of the graph
list<list<int> > SCC(vector<vector<int> >& neighbors) {
  int n_vertices = neighbors.size();
  stack<int> explored;
  vector<bool> visited(n_vertices, false);
  list<int> tmp;
  // dfs from each node
  for (unsigned i = 0; i < n_vertices; ++i) {
    if (!visited[i]) {
      tmp = dfs(i, visited, neighbors);
      for (std::list<int>::iterator i = tmp.begin(); i != tmp.end(); ++i)
        explored.push(*i);
    }
  }

  fill(visited.begin(), visited.end(), false);
  vector<vector<int> > neighbors_rev = rev_edges(n_vertices, neighbors);
  list<list<int> > components;
  list<int>* reachable;
  int curr;
  if (explored.size() != n_vertices)
    cerr << "wtf, we have " << explored.size() << ", " << n_vertices << endl;

  while (!explored.empty()) {
    curr = explored.top();
    explored.pop();
    if (!visited[curr]) {
      components.push_front(dfs(curr, visited, neighbors_rev));
    }
  }

  return components;
}

int main(int argc, char const* argv[]) {
  int n_vertices;
  cin >> n_vertices;
  cin.ignore();

  vector<vector<int> > neighbors = read_edges(n_vertices, cin);  // tested read

  list<list<int> > scc = SCC(neighbors);

  printSCC(scc);

  return 0;
}
