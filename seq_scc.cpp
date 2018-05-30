#include <fstream>
#include <iostream>
#include <list>
#include <sstream>
#include <stack>
#include <string>
#include <vector>

using namespace std;

// in reach are the nodes reachable by dfs from this node
void explore_rec(int current, stack<int>* explored, bool* visited,
                 vector<vector<int> > neighbors, list<int>* reach) {
  visited[current] = true;
  reach->push_front(current);
  for (vector<int>::iterator i = neighbors[current].begin();
       i != neighbors[current].end(); ++i) {
    if (!visited[*i]) {
      explore_rec(*i, explored, visited, neighbors, reach);
    }
  }

  explored->push(current);
  return;
}

// this will return the components of the graph
list<list<int> > explore(stack<int>* order, stack<int>* explored, bool* visited,
                         vector<vector<int> > neighbors) {
  list<list<int> > components;
  list<int>* reachable;
  int curr;
  while (!order->empty()) {
    curr = order->top();
    order->pop();
    if (!visited[curr]) {
      reachable = new list<int>();
      explore_rec(curr, explored, visited, neighbors, reachable);
      components.push_front(*reachable);
    }
  }
  return components;
}

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

vector<vector<int> > rev_edges(int n_vertices, vector<vector<int> > neighbors) {
  vector<vector<int> > neighbors_rev(n_vertices, vector<int>());
  for (unsigned i = 0; i < n_vertices; ++i) {
    for (vector<int>::iterator j = neighbors[i].begin();
         j != neighbors[i].end(); ++j) {
      neighbors_rev[*j].push_back(i);
    }
  }
  return neighbors_rev;
}

int main(int argc, char const* argv[]) {
  int n_vertices;
  cin >> n_vertices;
  cin.ignore();
  vector<vector<int> > neighbors = read_edges(n_vertices, cin);
  vector<vector<int> > neighbors_rev = rev_edges(n_vertices, neighbors);

  stack<int> explored;
  stack<int> order;

  bool* visited = new bool[n_vertices];

  for (unsigned i = 0; i < n_vertices; ++i) {
    order.push(n_vertices - i - 1);
  }

  explore(&order, &explored, visited, neighbors);

  for (unsigned i = 0; i < n_vertices; ++i) {
    visited[i] = false;
  }

  list<list<int> > scc = explore(&explored, &order, visited, neighbors_rev);

  for (std::list<list<int> >::iterator i = scc.begin(); i != scc.end(); ++i) {
    for (std::list<int>::iterator j = i->begin(); j != i->end(); ++j) {
      cout << *j << " ";
    }
    cout << endl;
  }

  return 0;
}
