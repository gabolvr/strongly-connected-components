#include <list>
#include <stack>
#include <iostream>

using namespace std;

void explore_rec(int current, stack<int>* explored, bool* visited, list<int>* neighbors){
    visited[current] = true;
    for (list<int>::iterator i = neighbors[current].begin(); i != neighbors[current].end(); ++i){
        if(!visited[*i]){
            cout << "\t" << *i << endl;
            explore_rec(*i,  explored, visited, neighbors);
        }//was not explored
    }

    explored->push(current);
}

void explore(stack<int>* order, stack<int>* explored, bool* visited, list<int>* neighbors){
    int curr;
    while(!order->empty()) {
        curr = order->top();
        order->pop();
        if(!visited[curr]){
            cout << curr << endl;
            explore_rec(curr, explored, visited, neighbors);
        }
    }
}


int main(int argc, char const *argv[]){
    int n_vertices, n_edges;
    cin >> n_vertices >> n_edges;

    list<int>* neighbors = new list<int>[n_vertices];
    list<int>* neighbors_rev = new list<int>[n_vertices];

    stack<int> explored;
    stack<int> order;

    bool* visited = new bool[n_vertices];

    int ori, dest;
    for(unsigned i = 0; i < n_edges; ++i) {
        cin >> ori >> dest;
        neighbors[ori].push_front(dest);
        neighbors_rev[dest].push_front(ori);
    }

    for(unsigned i = 0; i < n_vertices; ++i) {
        order.push(n_vertices-i-1);
    }

    explore(&order, &explored, visited, neighbors);
    cout << "first treat" << endl;
    for(unsigned i = 0; i < n_vertices; ++i) {
        visited[i] = false;
    }

    if(explored.empty())
        cout << "end" << endl;
    explore(&explored, &order, visited, neighbors_rev);
    cout << "end" << endl;
    return 0;
}
