CXX = g++ -std=gnu++0x
CXXFLAGS = -Wall -g -D LABELEDSINK_UNIT_TEST
INCLUDES = 
LIBS = -lbz2
OBJS = LabeledSink.o
PROGRAM = LabeledSink.out

all:$(PROGRAM)

$(PROGRAM): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ $(INCLUDES) $(LIBS) -o $(PROGRAM)

.cpp.o:
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(LIBS) -c $<

.PHONY: clean
clean:
	rm -f *o $(PROGRAM)
