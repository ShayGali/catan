CXX=clang++-9
CXXFLAGS=-std=c++2a -g -Wall

TIDY_FLAGS=-checks=bugprone-*,clang-analyzer-*,cppcoreguidelines-*,performance-*,portability-*,readability-*,-cppcoreguidelines-pro-bounds-pointer-arithmetic,-cppcoreguidelines-owning-memory,-readability-magic-numbers,-cppcoreguidelines-avoid-magic-numbers,-readability-isolate-declaration --warnings-as-errors=-* --

.PHONY: all clean tidy

all: KngihtCard.o MonopolyCard.o RoadBuildCard.o VictoryPointCard.o YearOfPlentyCard.o

%.o: %.cpp %.hpp Card.hpp
	$(CXX) $(CXXFLAGS) -c $<

tidy:
	clang-tidy *.cpp *.hpp $(TIDY_FLAGS)
clean:
	rm -f *.o