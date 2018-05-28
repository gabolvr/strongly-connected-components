#include <iostream>
#include <vector>
#include <unordered_set>
#include <string>
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
		push_back(i);

	if(task_id == 0){

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