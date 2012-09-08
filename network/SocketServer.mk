CXX = g++ -std=gnu++0x
CXXFLAGS = -Wall -g -D SOCKETSERVER_UNIT_TEST
INCLUDES = 
LIBS = -lboost_serialization -lboost_thread -ldl -lpthread -lboost_system
OBJS = SocketServer.o
PROGRAM = SocketServer.out

all:$(PROGRAM)

$(PROGRAM): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ $(INCLUDES) $(LIBS) -o $(PROGRAM)

.cpp.o:
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(LIBS) -c $<

.PHONY: clean
clean:
	rm -f *o $(PROGRAM)
