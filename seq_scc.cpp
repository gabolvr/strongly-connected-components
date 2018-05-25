#include <list>
#include <stack>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

void explore_rec(int current, stack<int>* explored, bool* visited, list<int>* neighbors, list<int>* reach){
    visited[current] = true;
    for (list<int>::iterator i = neighbors[current].begin(); i != neighbors[current].end(); ++i){
        if(!visited[*i]){
            explore_rec(*i,  explored, visited, neighbors, reach);
            reach->push_front(*i);
        }//was not explored
    }

    explored->push(current);
    return;
}

//this will return the components of the graph
list< list<int> > explore(stack<int>* order, stack<int>* explored, bool* visited, list<int>* neighbors){
    list< list<int> > components;
    list<int>* reachable;
    int curr;
    while(!order->empty()) {
        curr = order->top();
        order->pop();
        if(!visited[curr]){
            reachable = new list<int>();
            explore_rec(curr, explored, visited, neighbors, reachable);
            reachable->push_front(curr);
            components.push_front(*reachable);
        }
    }
    return components;
}


list<int>* read_edges(int n_vertices, istream& in){
    list<int>* neighbors = new list<int>[n_vertices];

    int dest;
    string line;
    for(unsigned i = 0; i < n_vertices; ++i) {
        getline(in, line);
        stringstream estream(line);
        while(estream >> dest){
            neighbors[i].push_front(dest);
        }
    }
    return neighbors;
}

list<int>* rev_edges(int n_vertices, list<int>* neighbors){
    list<int>* neighbors_rev = new list<int>[n_vertices];
    for(unsigned i = 0; i < n_vertices; ++i) {
        for (list<int>::iterator j = neighbors[i].begin(); j != neighbors[i].end(); ++j){
            neighbors_rev[*j].push_front(i);
        }
    }
    return neighbors_rev;
}

int main(int argc, char const *argv[]){
    int n_vertices;
    cin >> n_vertices >> std::ws;

    list<int>* neighbors = read_edges(n_vertices, cin);
    list<int>* neighbors_rev = rev_edges(n_vertices, neighbors);

    stack<int> explored;
    stack<int> order;

    bool* visited = new bool[n_vertices];

    for(unsigned i = 0; i < n_vertices; ++i) {
        order.push(n_vertices-i-1);
    }

    explore(&order, &explored, visited, neighbors);

    for(unsigned i = 0; i < n_vertices; ++i) {
        visited[i] = false;
    }

    list<list<int> > scc = explore(&explored, &order, visited, neighbors_rev);

    for (std::list<list<int> >::iterator i = scc.begin(); i != scc.end(); ++i){
        for (std::list<int>::iterator j = i->begin(); j != i->end(); ++j){
            cout << *j << " ";
        }
        cout << endl;
    }

    return 0;
}
