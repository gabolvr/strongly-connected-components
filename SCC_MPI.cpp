#include <iostream>
#include <vector>
#include <unordered_set>
#include <string>
#include <queue>
#include <sstream>
#include <fstream>
#include <mpi.h>

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

void DCSC_Sequential(vector<int> vertices, vector<unordered_set<int> >& graph_edges_out,
						vector<unordered_set<int> >& graph_edges_in, vector<vector<int> >& scc){
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

	DCSC_Sequential(s2, graph_edges_out, graph_edges_in, scc);
	DCSC_Sequential(s3, graph_edges_out, graph_edges_in, scc);
	DCSC_Sequential(s4, graph_edges_out, graph_edges_in, scc);

}

void DCSC_Rec(vector<int> vertices, int n_vertices, vector<unordered_set<int> >& graph_edges_out, vector<unordered_set<int> >& graph_edges_in){
	int task_id;
	MPI_Comm_rank(MPI_COMM_WORLD, &task_id);
	
	if(n_vertices == 0){
		// Send message saying that the work is over
		//cout << "proc " << task_id << " ends DCSC" << endl;
		MPI_Send(NULL, 0, MPI_INT, 0, 0, MPI_COMM_WORLD);
		return;
	}
	
	if(n_vertices == 1){
		// Send the connected component to the root
		//cout << "proc " << task_id << " sends a vector of size " << n_vertices << endl;
		MPI_Send(&vertices[0], n_vertices, MPI_INT, 0, 2, MPI_COMM_WORLD);
		// Send message saying that the work is over
		//cout << "proc " << task_id << " ends DCSC" << endl;
		MPI_Send(NULL, 0, MPI_INT, 0, 0, MPI_COMM_WORLD);
		return;
	}

	int start = vertices[rand() % n_vertices];

	unordered_set<int> succ = successors(graph_edges_out, start);
	unordered_set<int> pred = predecessors(graph_edges_in, start);

	// Find the four differents sets of verticex
	vector<int> s1, s2, s3, s4;
	int n1 = 0, n2 = 0, n3 = 0, n4 = 0;
	for(int i = 0; i < n_vertices; i++){
		if (succ.count(vertices[i]) > 0){
			if(pred.count(vertices[i]) > 0){
				s1.push_back(vertices[i]);
				n1++;
			}
			else{
				s2.push_back(vertices[i]);
				n2++;
			}
		}
		else if (pred.count(vertices[i]) > 0){
			s3.push_back(vertices[i]);
			n3++;
		}
		else{
			s4.push_back(vertices[i]);
			n4++;
		}
	}

	// Send the connected component to the root
	MPI_Send(&s1[0], n1, MPI_INT, 0, 2, MPI_COMM_WORLD);

	// Send the set of vertices to the root so that it allocates it to another processor
	MPI_Send(&s2[0], n2, MPI_INT, 0, 1, MPI_COMM_WORLD);
	// Send the set of vertices to the root so that it allocates it to another processor
	MPI_Send(&s3[0], n3, MPI_INT, 0, 1, MPI_COMM_WORLD);

	// Recursion in this set of vertices
	DCSC_Rec(s4, n4, graph_edges_out, graph_edges_in);
}

void DCSC(vector<int>* vertices, vector<unordered_set<int> >& graph_edges_out, vector<unordered_set<int> >& graph_edges_in, vector<vector<int> >& scc){
	int task_id, num_tasks, n_vertices, n_vertices_total, tag;
	bool working, free_procs = true;

	MPI_Comm_rank(MPI_COMM_WORLD, &task_id);
  	MPI_Comm_size(MPI_COMM_WORLD, &num_tasks);
  	MPI_Status status;

  	n_vertices_total = vertices->size();
  	n_vertices = n_vertices_total;
  	if(n_vertices > 0)
  		working = true;

  	tag = 1;

  	if(task_id == 0){
  		bool* working_procs = new bool[num_tasks];
  		for(int i = 1; i < num_tasks; i++)
  			working_procs[i] = false;

  		queue<vector<int>* > vertices_sets;
  		vertices_sets.push(vertices);

  		while(working){

  			// Send the set of vertices received to the first processor available
  			while(!vertices_sets.empty() && free_procs){
  				vertices = vertices_sets.front();
  				//vertices_sets.pop();
  				n_vertices = vertices->size();
  				if(n_vertices == 1){
  					scc.push_back(*vertices);
  					vertices_sets.pop();
  				}
  				else if(n_vertices > 1){
  					int free_task;
	  				for(free_task = 1; free_task < num_tasks && working_procs[free_task]; free_task++);
	  				
	  				if (free_task == num_tasks)
	  					free_procs = false;
	  				else{
	  					working_procs[free_task] = true;
	  					MPI_Send(&(*vertices)[0], n_vertices, MPI_INT, free_task, 1, MPI_COMM_WORLD);
	  					//cout << "proc " << task_id << " sends to proc " << free_task << " an array of size " << n_vertices << endl;
	  					//delete vertices;
	  					vertices_sets.pop();
	  				}
  				}
  				else{
  					vertices_sets.pop();
  					//delete vertices;
  				}
  			}
  			/*if(tag == 1){
  				if(n_vertices == 1){
  					scc.push_back(vector<int>());
  					scc[scc.size() - 1].push_back(vertices[0]);
  				}
  				else if(n_vertices > 1){
	  				int free_task;
	  				for(free_task = 1; free_task < num_tasks && working_procs[free_task]; free_task++);

	  				if(free_task < num_tasks){
	  					working_procs[free_task] = true;
	  					MPI_Send(&vertices[0], n_vertices, MPI_INT, free_task, 1, MPI_COMM_WORLD);
	  					cout << "proc " << task_id << " sends to proc " << free_task << " an array of size " << n_vertices << endl;
	  				}
	  				else{
	  					DCSC_Sequential(vertices, graph_edges_out, graph_edges_in, scc);
	  				}
  				}
  			}*/

  			// Receives the message from another task that is executing DCSC_Rec, which may be of 3 types :
  			// tag 0 : the task arrived at the end of the recursion and is now free
  			// tag 1 : receives a set of vertices to be allocated to another task
  			// tag 2 : receives a SCC and add it to the list
  			// Get information about the message before MPI_Recv
  			MPI_Probe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
  			MPI_Get_count(&status, MPI_INT, &n_vertices);
  			tag = status.MPI_TAG;
  			//cout << "proc " << task_id << " will receive from proc " << status.MPI_SOURCE << " a vector of size " << n_vertices << endl;
  			vertices = new vector<int>(n_vertices);
  			MPI_Recv(&(*vertices)[0], n_vertices, MPI_INT, status.MPI_SOURCE, tag, MPI_COMM_WORLD, &status);
  			//cout << "proc " << task_id << " received from proc " << status.MPI_SOURCE << " a vector of size " << n_vertices << endl;
  			if(tag == 0){
  				working_procs[status.MPI_SOURCE] = false;
  				free_procs = true;

  				working = false;
  				for(int i = 1; i < num_tasks && !working; i++){
  					if(working_procs[i])
  						working = true;
  				}
  				if (!vertices_sets.empty())
  					working = true;
  			}
  			else if(tag == 1){
  				vertices_sets.push(vertices);
  			}
  			else if(tag == 2){
  				scc.push_back(*vertices);
  				//cout << vertices->size() << " vetrices in SCC" << endl;
  				/*scc.push_back(vector<int>());
  				for(int i = 0; i < n_vertices; i++)
  					scc[scc.size() - 1].push_back(vertices[i]);*/
  			}
  		}

  		for(int i = 1; i < num_tasks; i++){
  			MPI_Send(&vertices[0], 1, MPI_INT, i, 0, MPI_COMM_WORLD);
  		}

  		cout << "size : " << vertices_sets.size() << endl;
  	}
  	else{
  		while(tag == 1){
  			MPI_Probe(0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
  			MPI_Get_count(&status, MPI_INT, &n_vertices);
  			vertices = new vector<int>(n_vertices);
  			MPI_Recv(&(*vertices)[0], n_vertices, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
  			tag = status.MPI_TAG;
  			//cout << "proc " << task_id << " receives from proc " << status.MPI_SOURCE << " a vector of size " << n_vertices << endl;
  			if(tag == 1){
  				//cout << "proc " << task_id << " starts DCSC" << endl;
  				DCSC_Rec(*vertices, n_vertices, graph_edges_out, graph_edges_in);
  				delete vertices;
  			}
  		}
  	}
}

vector<int> trim(vector<int> vertices, vector<unordered_set<int> >& graph_edges_out, vector<unordered_set<int> >& graph_edges_in, vector<vector<int> >& scc){
	queue<int> forward_trim, backward_trim;
	unordered_set<int> toDelete;
	int vertex;


	// forward trim
	for(int i = 0; i < vertices.size(); i++){
		if(graph_edges_in[i].empty()){
			forward_trim.push(i);
			toDelete.insert(i);
		}
	}

	while(!forward_trim.empty()){
		vertex = forward_trim.front();
		forward_trim.pop();
		
		// Add to SCC
		scc.push_back(vector<int>());
		scc[scc.size() - 1].push_back(vertex);
		
		// Delete out-edges
		for(unordered_set<int>::iterator it = graph_edges_out[vertex].begin(); it != graph_edges_out[vertex].end(); it++){
			graph_edges_in[*it].erase(vertex);
			if(graph_edges_in[*it].empty()){
				forward_trim.push(*it);
				toDelete.insert(*it);
			}
		}
		graph_edges_out[vertex].clear();
	}

	// backward trim

	for(int i = 0; i < vertices.size(); i++){
		if(graph_edges_out[i].empty() && toDelete.count(i) == 0){
			backward_trim.push(i);
			toDelete.insert(i);
		}
	}

	while(!backward_trim.empty()){
		vertex = backward_trim.front();
		backward_trim.pop();
		
		// Add to SCC
		scc.push_back(vector<int>());
		scc[scc.size() - 1].push_back(vertex);
		
		// Delete out-edges
		for(unordered_set<int>::iterator it = graph_edges_in[vertex].begin(); it != graph_edges_in[vertex].end(); it++){
			graph_edges_out[*it].erase(vertex);
			if(graph_edges_out[*it].empty()){
				backward_trim.push(*it);
				toDelete.insert(*it);
			}
		}
		graph_edges_in[vertex].clear();
	}

	// update vectors

	vector<int> new_vertices;
	for(int i = 0; i < vertices.size(); i++){
		if(toDelete.count(i) == 0)
			new_vertices.push_back(i);
	}

	return new_vertices;
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
	vector<vector<int> > scc, scc_seq;

	for(int i = 0; i < n_vertices; i++)
		vertices.push_back(i);

	vertices = trim(vertices, graph_edges_out, graph_edges_in, scc);
	if(task_id == 0){
		cout << n_vertices << " vertices" << endl << endl;
		cout << "Trim : " << vertices.size() << " vertices" << endl << endl;
	}

	DCSC(&vertices, graph_edges_out, graph_edges_in, scc);
	MPI_Barrier(MPI_COMM_WORLD);

	if(task_id == 0){
		cout << "Parallel" << endl;
		int nv = 0;
		float cluster_coef, average_cluster_coef = 0;
		for(int i = 0; i < scc.size(); i++){
			cluster_coef = (1.0 * scc[i].size()) / n_vertices ;
			average_cluster_coef += cluster_coef;
			for(int j = 0; j < scc[i].size(); j++, nv++)
				cout << scc[i][j] << " ";
			cout << endl;
		}
		average_cluster_coef /= scc.size();
		cout << endl << "Vertices : " << nv << endl << "SCC : " << scc.size() << endl;
		cout << "Average clustering coefficient : " << average_cluster_coef << endl;

		/*cout << endl << "Sequential" << endl;
		nv = 0;
		DCSC_Sequential(vertices, graph_edges_out, graph_edges_in, scc_seq);
		for(int i = 0; i < scc_seq.size(); i++){
			for(int j = 0; j < scc_seq[i].size(); j++, nv++)
				cout << scc_seq[i][j] << " ";
			cout << endl;
		}
		cout << endl << "Vertices : " << nv << endl << "SCC : " << scc_seq.size() << endl;
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