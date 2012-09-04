CXX = g++ -std=gnu++0x
CXXFLAGS = -Wall -g -D KEYHASHDB_UNIT_TEST
INCLUDES = 
LIBS = -lcrypto
OBJS = KeyHashDb.o
PROGRAM = KeyHashDb.out

all:$(PROGRAM)

$(PROGRAM): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ $(INCLUDES) $(LIBS) -o $(PROGRAM)

.cpp.o:
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(LIBS) -c $<

.PHONY: clean
clean:
	rm -f *o $(PROGRAM)
