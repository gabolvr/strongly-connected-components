SQ_SCC = seq_scc
TESTRW = test_read_write
ER_MPI = ER_graph_mpi
ER_SEQ = ER_graph_seq

HFILES=$(wildcard *.hpp)
OBJECTS = data2graph.o translator.o graph2out.o

CXX = mpic++ -std=c++11

all : $(SQ_SCC) $(TESTRW) $(ER_MPI) $(ER_SEQ)

$(SQ_SCC) : $(OBJECTS) $(SQ_SCC).o
	$(CXX) $(INCLUDES) -o $@ $^ $(LIBS)

$(ER_MPI) : $(OBJECTS) $(ER_MPI).o
	$(CXX) $(INCLUDES) -o $@ $^ $(LIBS)

$(ER_SEQ) : $(OBJECTS) $(ER_SEQ).o
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
