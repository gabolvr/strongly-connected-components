#include <fstream>
#include <iostream>
#include <list>
#include <random>
#include <string>
#include "graph2out.hpp"
#include "mpi.h"

using namespace std;

// create a erdos-renyi graph with n_vertices and probability for an edge = p
int* ER_graph(double p, int n_vertices, int start_v, int end_v) {
  int* edges = new int[(end_v - start_v) * n_vertices];
  double r;
  for (unsigned i = start_v; i < end_v; ++i) {
    for (unsigned j = 0; j < n_vertices; ++j) {
      r = double(rand()) / RAND_MAX;
      if (r < p && i != j) edges[(i - start_v) * n_vertices + j] = 1;
    }
  }
  return edges;
}
// we generate rounded_up n_vertices and then
// pick only the amount we need, so we don`t have problems
// partitioning
list<int>* ER_graph_mpi(int n_vertices, double p) {
  int num_tasks, task_id;
  MPI_Comm_rank(MPI_COMM_WORLD, &task_id);
  MPI_Comm_size(MPI_COMM_WORLD, &num_tasks);

  // partitioning the input
  int size_partit = (n_vertices / num_tasks) + 1;
  int round_n_vertices = size_partit * num_tasks;
  int start_local = size_partit * task_id;
  // obs: c++ is ok with new int[0]

  // each node creates its part of the graph
  int* edges_local =
      ER_graph(p, n_vertices, start_local, start_local + size_partit);

  // We gather them all in node zero
  int* edges_final = nullptr;
  if (task_id == 0) {
    edges_final = new int[round_n_vertices * n_vertices];
  }
  MPI_Gather(edges_local, (size_partit)*n_vertices, MPI_INT, edges_final,
             (size_partit)*n_vertices, MPI_INT, 0, MPI_COMM_WORLD);

  // we turn the binary information into a list of edges
  list<int>* edges = nullptr;
  if (task_id == 0) {
    edges = new list<int>[n_vertices];
    for (unsigned i = 0; i < n_vertices; ++i) {
      for (unsigned j = 0; j < n_vertices; ++j) {
        if (edges_final[i * n_vertices + j] == 1) edges[i].push_front(j);
      }
    }
  }

  return edges;
}

int main(int argc, char* argv[]) {
  int rc, task_id;
  rc = MPI_Init(&argc, &argv);
  if (rc != MPI_SUCCESS) {
    cout << "Error starting MPI program. Terminating.\n";
    MPI_Abort(MPI_COMM_WORLD, rc);
  }

  MPI_Comm_rank(MPI_COMM_WORLD, &task_id);

  // Checks the number of input arguments
  if (argc < 4) {
    cout << "./ER_graph_mpi \"n_vertices\" \"probability\" \"filename\""
         << endl;
    return -1;
  }

  const int n_vertices = stoi(argv[1]);
  const double p = stod(argv[2]);
  const string filename = argv[3];
  srand(time(0));  // really random

  list<int>* edges = ER_graph_mpi(n_vertices, p);
  if (task_id == 0) {
    graph2out gen_g(filename);
    gen_g.write_graph(edges, n_vertices);
  }

  MPI_Finalize();

  return 0;
}
