SQ_SCC = seq_scc
TESTRW = dgraph2rgraph
ER_MPI = ER_graph_mpi
ER_SEQ = ER_graph_seq
SCC_MPI = SCC_MPI
U2D = undirect_to_direct

HFILES=$(wildcard *.hpp)
OBJECTS = data2graph.o translator.o graph2out.o

CXX = mpic++ -std=c++11

all : $(SQ_SCC) $(TESTRW) $(ER_MPI) $(ER_SEQ) $(SCC_MPI) $(U2D)

$(U2D) : $(OBJECTS) $(U2D).o
	$(CXX) $(INCLUDES) -o $@ $^ $(LIBS)

$(SQ_SCC) : $(OBJECTS) $(SQ_SCC).o
	$(CXX) $(INCLUDES) -o $@ $^ $(LIBS)

$(ER_MPI) : $(OBJECTS) $(ER_MPI).o
	$(CXX) $(INCLUDES) -o $@ $^ $(LIBS)

$(ER_SEQ) : $(OBJECTS) $(ER_SEQ).o
	$(CXX) $(INCLUDES) -o $@ $^ $(LIBS)

$(SCC_MPI) : $(OBJECTS) $(SCC_MPI).o
	$(CXX) $(INCLUDES) -o $@ $^ $(LIBS)

$(TESTRW) : $(OBJECTS) $(TESTRW).o
	$(CXX) $(INCLUDES) -o $@ $^ $(LIBS)

%o: %.cpp
	$(CXX) -c $< -o $@

.PHONY: clean

clean :
	rm -f $(SQ_SCC) $(ER_MPI) $(TESTRW) $(ER_SEQ)
	rm -f *.o *.out
	./sanitize.sh
