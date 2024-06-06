CXX=clang++-9
CXXFLAGS=-std=c++2a -g -Wall

GAME_PIECE_OBJECTS=../game_piece/LandVertex.o ../game_piece/RoadEdge.o
PLAYER_OBJECTS=../player/Player.o
CARDS_OBJECTS=../cards/KngihtCard.o ../cards/MonopolyCard.o ../cards/RoadBuildCard.o ../cards/VictoryPointCard.o ../cards/YearOfPlentyCard.o

GAME=../Catan.o

all: test

test: $(PROG) test.o $(GAME) $(GAME_PIECE_OBJECTS) $(PLAYER_OBJECTS) $(CARDS_OBJECTS)
	make -C ../ catan
	$(CXX) $(CXXFLAGS) -o ./test test.o $(GAME) $(GAME_PIECE_OBJECTS) $(PLAYER_OBJECTS) $(CARDS_OBJECTS)

run: test
	./test

valgrind: test
	valgrind $(VALGRIND_FLAGS) ./$< 2>&1

clean:
	rm -f test test.o

.PHONY: run