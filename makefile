CXX=clang++-9
CXXFLAGS=-std=c++2a -g -Wall

GAME_PIECE_OBJECTS=game_piece/LandVertex.o game_piece/RoadEdge.o
PLAYER_OBJECTS=player/Player.o
CARDS_OBJECTS=

PROG=catan

.PHONY: all clean run game_piece player cards catan

run: ./$(PROG)
	./$(PROG)

catan: Catan.o main.o game_piece player
	$(CXX) $(CXXFLAGS) -o catan Catan.o main.o $(GAME_PIECE_OBJECTS) $(PLAYER_OBJECTS) $(CARDS_OBJECTS)

Catan.o: Catan.cpp
	$(CXX) $(CXXFLAGS) -c Catan.cpp

main.o: main.cpp
	$(CXX) $(CXXFLAGS) -c main.cpp

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $<

game_piece:
	make -C game_piece all

player:
	make -C player all

# cards:
# 	make -C cards all

clean:
	rm -f *.o $(PROG)
	make -C game_piece clean
	make -C player clean
	# make -C cards clean
