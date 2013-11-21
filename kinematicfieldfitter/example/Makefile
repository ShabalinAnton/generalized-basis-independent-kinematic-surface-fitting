CXX := g++

CXXFLAGS := -I../include -I../src
CXXFLAGS_DEBUG := -Wall -g 
CXXFLAGS_RELEASE := -O3 

LDFLAGS := -llapack -lblas -L../ -lkff

# List of the common source files
objs=kff_example.o 
src=kff_example.cpp 

# Makefile rules
all: kff_example

kff_example: $(objs)
	$(CXX) -o $@ $^ $(CXXFLAGS) $(LDFLAGS)

kff_example.o: kff_example.cpp 
	$(CXX) $(CXXFLAGS) -c kff_example.cpp

clean:
	rm -f $(objs) kff_example
