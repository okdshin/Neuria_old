CXX = ccache g++ -std=gnu++0x
CXXFLAGS = -Wall -g -D P2PCORE_UNIT_TEST -D DEBUG_MODE -D DEBUG_LEVEL=40
INCLUDES = -I/usr/include 
LIBS = -lboost_serialization -lboost_thread -ldl -lpthread -lboost_system
OBJS = P2pCore.o
PROGRAM = P2pCore.out

all:$(PROGRAM)

$(PROGRAM): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ $(INCLUDES) $(LIBS) -o $(PROGRAM)

.cpp.o:
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(LIBS) -c $<

.PHONY: clean
clean:
	rm -f *o $(PROGRAM)
