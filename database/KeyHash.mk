CXX = g++ -std=gnu++0x
CXXFLAGS = -Wall -g -D KEYHASH_UNIT_TEST
INCLUDES = 
LIBS = -lboost_system -lboost_thread -lboost_iostreams -lboost_serialization
OBJS = KeyHash.o
PROGRAM = KeyHash.out

all:$(PROGRAM)

$(PROGRAM): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ $(INCLUDES) $(LIBS) -o $(PROGRAM)

.cpp.o:
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(LIBS) -c $<

.PHONY: clean
clean:
	rm -f *o $(PROGRAM)
