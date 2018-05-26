# user-definable settings
TRIVIAL =      # 1=YES, [empty] = no
DEBUG =        # 1=YES, [empty] = no
SETUP = ubuntu # possibilities: macosx, ubuntu, sallesinfo

# defines and compiler
DEFINES = -std=c++11
CXX = c++

ifdef TRIVIAL
  DEFINES += -DTRIVIAL
endif

# flags
FLAGS = $(DEFINES)
ifdef DEBUG
  FLAGS += -g
else
  FLAGS +=
endif

ifeq "$(SETUP)" "sallesinfo "
  ## salles informatiques @X
  $(info building for $(SETUP))
  INCLUDE = -I/usr/local/boost-1.65.0/include/ -I/usr/local/CImg-2.2.2/
  LIBS = -lpthread -lX11 -L/usr/local/boost-1.65.0/lib/
  LDPATH = LD_LIBRARY_PATH=/usr/local/boost-1.65.0/lib:/usr/lib/alliance/lib
endif

ifeq "$(SETUP)" "ubuntu "
  # ubuntu linux
  $(info building for $(SETUP))
  INCLUDE =
  LIBS =
  LDPATH =
endif



# name of executable
BIN = ER_graph_seq

# object files it depends on
OBJS = graph2out.o

# remove command
RM = rm -f

# default rule - typing "make" will start this rule
#   the dependency is on Makefile itself (so if the Makefile was changed
#   the whole project is rebuilt) and on the executable
all:	$(BIN) Makefile

# rule to build the executable
$(BIN): $(OBJS) $(BIN).cpp Makefile
	$(CXX) $(FLAGS) $(INCLUDE) -o $(BIN) $(BIN).cpp $(OBJS) $(LIBS)

# clean objects and executable
clean:
	$(RM) $(OBJS) $(BIN) box_muller cimgvec k_means

# restore directory to pristine state
distclean: clean
	$(RM) core *~

# catch-all rule to compile object files
#   $@ refers to the rule name (.o) and %< to the first item in rule (.cpp)
%.o:	%.cpp %.hpp Makefile
	$(CXX) $(FLAGS) $(INCLUDE) -c -o $@ $<
