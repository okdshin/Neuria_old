CXX = g++ -std=gnu++0x
CXXFLAGS = -Wall -g -D FILEKEYHASH_UNIT_TEST
INCLUDES = 
LIBS = -lboost_date_time -lboost_filesystem -lcrypt -lboost_system -lboost_thread -lboost_iostreams -lboost_serialization -lpthread -lcrypto -lboost_serialization -lboost_thread -ldl -lpthread -lboost_system
OBJS = FileKeyHash.o
PROGRAM = FileKeyHash.out

all:$(PROGRAM)

$(PROGRAM): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ $(INCLUDES) $(LIBS) -o $(PROGRAM)

.cpp.o:
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(LIBS) -c $<

.PHONY: clean
clean:
	rm -f *o $(PROGRAM)
