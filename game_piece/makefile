# shay.gali@msmail.ariel.ac.il
CXX=clang++-9
CXXFLAGS=-std=c++2a -g -Wall

TIDY_FLAGS=-checks=bugprone-*,clang-analyzer-*,cppcoreguidelines-*,performance-*,portability-*,readability-*,-cppcoreguidelines-pro-bounds-pointer-arithmetic,-cppcoreguidelines-owning-memory,-readability-magic-numbers,-cppcoreguidelines-avoid-magic-numbers,-readability-isolate-declaration --warnings-as-errors=-* --

.PHONY: all clean tidy

all: LandVertex.o RoadEdge.o

LandVertex.o: LandVertex.cpp LandVertex.hpp resource.hpp
	$(CXX) $(CXXFLAGS) -c LandVertex.cpp

RoadEdge.o: RoadEdge.cpp RoadEdge.hpp resource.hpp
	$(CXX) $(CXXFLAGS) -c RoadEdge.cpp

tidy:
	clang-tidy *.cpp *.hpp $(TIDY_FLAGS)

clean:
	rm -f *.o