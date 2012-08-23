CXX = ccache g++ -std=gnu++0x
CXXFLAGS = -Wall -g -D P2PNODE_UNIT_TEST
INCLUDES = -I/usr/include 
LIBS = -lboost_serialization -lboost_thread -ldl -lpthread -lboost_system
OBJS = P2pNode.o
PROGRAM = P2pNode.out

all:$(PROGRAM)

$(PROGRAM): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ $(INCLUDES) $(LIBS) -o $(PROGRAM)

.cpp.o:
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(LIBS) -c $<

.PHONY: clean
clean:
	rm -f *o $(PROGRAM)
