CXX = ccache g++ -std=gnu++0x
CXXFLAGS = -Wall -g -D FUNC_MANAGER_UNIT_TEST
INCLUDES = 
LIBS = -lboost_serialization -lboost_thread -ldl -lpthread -lboost_system
OBJS = FuncManager.o
PROGRAM = FuncManager.out

all:$(PROGRAM)

$(PROGRAM): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ $(INCLUDES) $(LIBS) -o $(PROGRAM)

.cpp.o:
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(LIBS) -c $<

.PHONY: clean
clean:
	rm -f *o $(PROGRAM)
