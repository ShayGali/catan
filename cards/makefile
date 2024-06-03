CXX=clang++-9
CXXFLAGS=-std=c++2a -g -Wall

.PHONY: all clean

all: KngihtCard.o MonopolyCard.o RoadBuildCard.o VictoryPointCard.o YearOfPlentyCard.o

%.o: %.cpp %.hpp Card.hpp
	$(CXX) $(CXXFLAGS) -c $<

clean:
	rm -f *.o