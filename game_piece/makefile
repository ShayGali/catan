CXX=clang++-9
CXXFLAGS=-std=c++2a -g -Wall

.PHONY: all clean

all: LandVertex.o RoadEdge.o

LandVertex.o: LandVertex.cpp LandVertex.hpp resource.hpp
	$(CXX) $(CXXFLAGS) -c LandVertex.cpp

RoadEdge.o: RoadEdge.cpp RoadEdge.hpp resource.hpp
	$(CXX) $(CXXFLAGS) -c RoadEdge.cpp

clean:
	rm -f *.o