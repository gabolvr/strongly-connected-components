# Finding Strongly Connected Components

The goal of this project is to implement a sequential and a parallel program that identifies the strongly connected components of a directed graph (*digraphs*), as well as to generate Erdõs-Rényi (ER) digraphs.

Programming project for the course INF442 (*Traitement des données massives*) at École Polytechnique.


## Quick Description
  1. ER_graph: Generates an Erdos-Renyi graph. \_seq stands for sequential and \_mpi for using openMPI 
  2. format_graph.txt: explains the format used for representing the graph in a file
  3. \*.er: files with parameters. *Lazyness forever* Used in ER_graph and test
  4. sanitize.sh: cleans the directory, removing any randomly generated graphs
  5. seq_scc: sequential implementation of the Kosaraju's algorithm to find Strongly Connected components
  6. \*.graph: example of graph files
  7. \*.rgraph: generated graphs, from module graph2out, their format is explained in format_graph.txt
  8. \*.dgraph: data graphs, download them from SNAP data base. Their format is basically a list of edges
  9. graph2out: picks a graph and write it in a .rgraph file
  10. data2graph: picks a .dgraph file and creates a graph from it
  11. Makefile, temp_make: first is for the sequential part and seconde is for the parallel part
 
