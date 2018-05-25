#include <iostream>
#include <list>
#include <random>
#include <string>
#include <fstream>

using namespace std;

const string EXTENSION = ".rgraph";//extension for random graph, used by sanitizer

//create a erdos-renyi graph with n_vertices and probability for an edge = p
list<int>* ER_graph(double p, int n_vertices){
    list<int>* edges = new list<int>[n_vertices];
    double r;
    for(unsigned i = 0; i < n_vertices; ++i) {
        for(unsigned j = 0; j < n_vertices; ++j) {
            r = double(rand())/RAND_MAX;
            if(r < p && i != j)
                edges[i].push_front(j);
        }
    }
    return edges;
}

//general function that picks a graph and writes it, in the format "format_graph.txt", in the stream
void graph2stream(list<int>* edges, int n_vertices, ostream& out_text){
    out_text << n_vertices << endl;
    for(unsigned vert = 0; vert < n_vertices; ++vert) {
        for (list<int>::iterator dest = edges[vert].begin(); dest != edges[vert].end(); ++dest){
             out_text << (*dest) << " ";//problem, last one has a space
        }
        out_text << endl;
    }
    return;
}

//if you want to output the graph to a file use this
void graph2file(list<int>* edges, int n_vertices, string out_base_name){
    ofstream out_text;
    out_text.open(out_base_name + EXTENSION);

    graph2stream(edges, n_vertices, out_text);

    out_text.close();
    return;
}

int main(int argc, char const *argv[]){

    //picking parameters
    cout << "Hello, this module generates erdos-renyi graphs!\nPlease specify (n_vertices, prob_of_an_edge)\n";
    int n_vertices;
    double p;
    cin >> n_vertices >> p;

    srand(time(0));//really random
    list<int>* edges = ER_graph(p, n_vertices);

    cout << "\nIf you want to output the graph in a file type the name you want (no extension).\nOtherwise press Enter\n";
    string out_base_name;
    cin.ignore();
    getline(cin, out_base_name);
    if(out_base_name == ""){
        graph2stream(edges, n_vertices, cout);
    }
    else
        graph2file(edges, n_vertices, out_base_name);


    return 0;
}
