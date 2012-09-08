CXX = g++ -std=gnu++0x
CXXFLAGS = -Wall -g -D SOCKETCLIENT_UNIT_TEST -O0
INCLUDES = 
LIBS = -lboost_serialization -lboost_thread -ldl -lpthread -lboost_system
OBJS = SocketClient.o
PROGRAM = SocketClient.out

all:$(PROGRAM)

$(PROGRAM): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ $(INCLUDES) $(LIBS) -o $(PROGRAM)

.cpp.o:
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(LIBS) -c $<

.PHONY: clean
clean:
	rm -f *o $(PROGRAM)
