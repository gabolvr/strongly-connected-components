# Finding Strongly Connected Components

The goal of this project is to implement a sequential and a parallel program that identifies the strongly connected components of a directed graph (*digraphs*), as well as to generate Erdõs-Rényi (ER) digraphs.

Programming project for the course INF442 (*Traitement des données massives*) at École Polytechnique.


## Quick Description
  1. ER_graph: Generates an Erdos-Renyi graph. \_seq stands for sequential and \_mpi for using openMPI 
  2. format_graph.txt: explains the format used for representing the graph in a file
  3. project.pdf: description of the project
  4. sanitize.sh: cleans the directory, removing any randomly generated graphs
  5. seq_scc: sequential implementation of the Kosaraju's algorithm to find Strongly Connected components
  6. \*.graph: example of graph files
  7. \*.rgraph: generated graphs, from module graph2out, their format is explained in format_graph.txt
  8. \*.dgraph: data graphs, download them from SNAP data base. Their format is basically a list of edges
  9. graph2out: picks a graph and write it in a .rgraph file
  10. data2graph: picks a .dgraph file and creates a graph from it
  12. Makefile: first is for the sequential part. make clean runs sanitize.sh as well
  13. undirect_to_direct: Util for turning an undirect graph into direct. Picks argument from terminal
  14. SCC_MPI: Parallel solution to find the SCC, uses FW-BW-Trim algorithm
  15. dgraph2rgraph: turns a .dgraph into a .rgraph. Picks argument from terminal
  16. verify_complete: utils for debugging
  
## Quick Use Guide
  First of all
  1. Dowload everything ;)
  2. On the terminal go to the directory and tap make
  
  Options
  1. Generate ER graph:
      * Sequentially: run ./ER_graph_seq and follow instructions
      * Parallel: run it using mpi and giving it 3 arguments: n_vertices probability filename
      * Obs.: every filename you give shouldn't have extensions, we take care of that
  2. Pre-treat data from SNAP:
      * Download it from: http://snap.stanford.edu/data/index.html
      * Put it in the directory AND change its extension to .dgraph
      * run ./dgraph2rgraph filename, where filename is the name of the file without extension
  3. Find Strongly Connected Components:
      * Sequentially: run ./seq_scc filename, attention there must exist a filename.rgraph
      * Parallel: run ./SCC_MPI "filename.rgraph", in this case we to write the .rgraph [TODO]
    
    
