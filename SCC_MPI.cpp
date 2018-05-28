#include <iostream>
#include <vector>
#include <unordered_set>
#include <string>
#include <queue>
#include <sstream>
#include <fstream>
#include "mpi.h"

using namespace std;

// Generate the edges for each vertex from the source file
vector<unordered_set<int> > readGraph(string filename, int& n_vertices){
	ifstream graph_file;
	graph_file.open(filename);

	string line;
	int dest;

	graph_file >> n_vertices;
	getline(graph_file, line);
	vector<unordered_set<int> > graph_edges_out(n_vertices);

	for(int i = 0; i < n_vertices; i++){
		getline(graph_file, line);
    	stringstream estream(line);
    	while (estream >> dest)
      		graph_edges_out[i].insert(dest);
	}

	graph_file.close();

	return graph_edges_out;
}

// Generate the edges entering each vertex from the vector with exiting edges
vector<unordered_set<int> > generateEdgesIn(vector<unordered_set<int> >& graph_edges_out){
	int n_vertices = graph_edges_out.size();
	vector<unordered_set<int> > graph_edges_in(n_vertices);

	for(int i = 0; i < n_vertices; i++){
		for(unordered_set<int>::iterator it = graph_edges_out[i].begin(); it != graph_edges_out[i].end(); it++)
			graph_edges_in[*it].insert(i);
	}

	return graph_edges_in;
}

unordered_set<int> successors(vector<unordered_set<int> >& graph_edges_out, int start){
	unordered_set<int> succ;
	queue<int> toVisit;
	int node;
	
	succ.insert(start);
	toVisit.push(start);

	while(!toVisit.empty()){
		node = toVisit.front();
		toVisit.pop();

		for(unordered_set<int>::iterator it = graph_edges_out[node].begin(); it != graph_edges_out[node].end(); it++){
			if (succ.count(*it) == 0){
				succ.insert(*it);
				toVisit.push(*it);
			}
		}
	}

	return succ;
}

unordered_set<int> predecessors(vector<unordered_set<int> >& graph_edges_in, int start){
	unordered_set<int> pred;
	queue<int> toVisit;
	int node;
	
	pred.insert(start);
	toVisit.push(start);

	while(!toVisit.empty()){
		node = toVisit.front();
		toVisit.pop();

		for(unordered_set<int>::iterator it = graph_edges_in[node].begin(); it != graph_edges_in[node].end(); it++){
			if (pred.count(*it) == 0){
				pred.insert(*it);
				toVisit.push(*it);
			}
		}
	}

	return pred;
}

void DCSC(vector<int> vertices, vector<unordered_set<int> >& graph_edges_out, vector<unordered_set<int> >& graph_edges_in, vector<vector<int> >& scc){
	if(vertices.size() == 0)
		return;
	
	if(vertices.size() == 1){
		scc.push_back(vector<int>());
		scc[scc.size() - 1].push_back(vertices[0]);
		return;
	}

	int start = vertices[rand() % vertices.size()];

	unordered_set<int> succ = successors(graph_edges_out, start);
	unordered_set<int> pred = predecessors(graph_edges_in, start);

	vector<int> s1, s2, s3, s4;
	for(int i = 0; i < vertices.size(); i++){
		if (succ.count(vertices[i]) > 0){
			if(pred.count(vertices[i]) > 0)
				s1.push_back(vertices[i]);
			else
				s2.push_back(vertices[i]);
		}
		else if (pred.count(vertices[i]) > 0)
			s3.push_back(vertices[i]);
		else
			s4.push_back(vertices[i]);
	}

	scc.push_back(s1);

	DCSC(s2, graph_edges_out, graph_edges_in, scc);
	DCSC(s3, graph_edges_out, graph_edges_in, scc);
	DCSC(s4, graph_edges_out, graph_edges_in, scc);

}

int main(int argc, char* argv[]){
	int rc, task_id, num_tasks;

	// Initializes MPI
	rc = MPI_Init(&argc, &argv);
  	if (rc != MPI_SUCCESS) {
    	cout << "Error starting MPI program. Terminating.\n";
    	MPI_Abort(MPI_COMM_WORLD, rc);
  	}

  	MPI_Comm_rank(MPI_COMM_WORLD, &task_id);
  	MPI_Comm_size(MPI_COMM_WORLD, &num_tasks);

  	// Checks the number of input arguments
  	if (argc < 2) {
    	cout << "./SCC_MPI \"graph_file.graph\" " << endl;
   		return -1;
  	}

  	string filename = argv[1];
  	int n_vertices;

	vector<unordered_set<int> > graph_edges_out = readGraph(filename, n_vertices);
	vector<unordered_set<int> > graph_edges_in = generateEdgesIn(graph_edges_out);

	vector<int> vertices;
	vector<vector<int> > scc;

	for(int i = 0; i < n_vertices; i++)
		vertices.push_back(i);

	if(task_id == 0){
		DCSC(vertices, graph_edges_out, graph_edges_in, scc);
		for(int i = 0; i < scc.size(); i++){
			for(int j = 0; j < scc[i].size(); j++)
				cout << scc[i][j] << " ";
			cout << endl;
		}
		/*cout << n_vertices << " n_vertices" << endl;
		for(int i = 0; i < n_vertices; i++){
			cout << i << " : ";
			for(unordered_set<int>::iterator it = graph_edges_out[i].begin(); it != graph_edges_out[i].end(); it++)
				cout << *it << " ";
			cout << endl;
		}
		cout << endl;
		for(int i = 0; i < n_vertices; i++){
			cout << i << " : ";
			for(unordered_set<int>::iterator it = graph_edges_in[i].begin(); it != graph_edges_in[i].end(); it++)
				cout << *it << " ";
			cout << endl;
		}*/
	}

  	// Terminate MPI.
    MPI_Finalize();

  	return 0;
}