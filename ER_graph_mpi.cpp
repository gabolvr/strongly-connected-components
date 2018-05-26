#include <fstream>
#include <iostream>
#include <list>
#include <random>
#include <string>
#include "graph2out.hpp"
#include "mpi.h"

using namespace std;

// create a erdos-renyi graph with n_vertices and probability for an edge = p
int** ER_graph(double p, int n_vertices, int start_v, int end_v) {
  int** edges = new int*[end_v - start_v];
  for (unsigned i = 0; i < end_v - start_v; ++i) {
    edges[i] = new int[n_vertices];
  }
  double r;
  for (unsigned i = start_v; i < end_v; ++i) {
    for (unsigned j = 0; j < n_vertices; ++j) {
      r = double(rand()) / RAND_MAX;
      if (r < p && i != j) edges[i][j] = 1;
    }
  }
  return edges;
}

list<int>* ER_graph_mpi(int n_vertices, double p) {
  int num_tasks, task_id;
  MPI_Comm_rank(MPI_COMM_WORLD, &task_id);
  MPI_Comm_size(MPI_COMM_WORLD, &num_tasks);
  int rest = n_vertices % num_tasks;
  int size_base = n_vertices / num_tasks;
  int start_local, end_local;
  if (task_id < rest) {
    start_local = task_id * (size_base + 1);
    end_local = start_local + (size_base + 1);
  } else {
    start_local = (task_id - rest) * size_base + rest * (size_base + 1);
    end_local = start_local + size_base;
  }
  int** edges_local = ER_graph(p, n_vertices, start_local, end_local);
  int** edges_final;
  if (task_id == 0) {
    edges_final = new int*[n_vertices];
    for (unsigned i = 0; i < n_vertices; ++i) {
      edges_final[i] = new int[n_vertices];
    }
  }
  MPI_Gather(edges_local, (end_local - start_local) * n_vertices, MPI_INT,
             edges_final, (end_local - start_local) * n_vertices, MPI_INT, 0,
             MPI_COMM_WORLD);
  list<int>* edges = nullptr;
  if (task_id == 0) {
    edges = new list<int>[n_vertices];
    for (unsigned i = 0; i < n_vertices; ++i) {
      for (unsigned j = 0; j < n_vertices; ++j) {
        if (edges_final[i][j]) edges[i].push_front(j);
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

  return 0;
}
