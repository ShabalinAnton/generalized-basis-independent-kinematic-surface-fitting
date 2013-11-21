CXX := g++

CXXFLAGS := -Iinclude -Ilib/CLAPACK/include
CXXFLAGS_DEBUG := -Wall -g 
CXXFLAGS_RELEASE := -O3 

LDFLAGS := -llapack -lblas



# List of the common source files
objs=kinematicfieldfit.o
src=src/kinematicfieldfit.cpp

# Makefile rules
all: libkff.a

libkff.a: $(objs)
	rm -f libkff.a
	ar rs libkff.a $^

kinematicfieldfit.o: src/kinematicfieldfit.cpp
	$(CXX) $(CXXFLAGS) -c $<


clean:
	rm -f $(objs) libkff.a
