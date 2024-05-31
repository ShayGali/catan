CXX=clang++-9
CXXFLAGS=-std=c++2a -g -Wall

.PHONY: all clean

all: Player.o

Player.o: Player.cpp Player.hpp ../game_piece/resource.hpp
	$(CXX) $(CXXFLAGS) -c Player.cpp

clean:
	rm -f *.o