CXX=clang++-9
CXXFLAGS=-std=c++2a -g -Wall

GAME_PIECE_OBJECTS=game_piece/LandVertex.o game_piece/RoadEdge.o
PLAYER_OBJECTS=player/Player.o
CARDS_OBJECTS=cards/KngihtCard.o cards/MonopolyCard.o cards/RoadBuildCard.o cards/VictoryPointCard.o cards/YearOfPlentyCard.o

VALGRIND_FLAGS=--tool=memcheck -v --leak-check=full --show-leak-kinds=all  --error-exitcode=99
TIDY_FLAGS=-checks=bugprone-*,clang-analyzer-*,cppcoreguidelines-*,performance-*,portability-*,readability-*,-cppcoreguidelines-pro-bounds-pointer-arithmetic,-cppcoreguidelines-owning-memory,-readability-magic-numbers,-cppcoreguidelines-avoid-magic-numbers,-readability-isolate-declaration,-cppcoreguidelines-pro-bounds-constant-array-index --warnings-as-errors=-* --

PROG=catan

.PHONY: clean run tidy test valgrind catan game_piece player cards

catan: Catan.o main.o game_piece player cards
	$(CXX) $(CXXFLAGS) -o catan Catan.o main.o $(GAME_PIECE_OBJECTS) $(PLAYER_OBJECTS) $(CARDS_OBJECTS)

run: $(PROG)
	./$(PROG)

debug: $(PROG)
	gdb $(PROG)

test:
	make -C tests run
	
valgrind: $(PROG)
	valgrind $(VALGRIND_FLAGS) ./$< 2>&1

tidy:
	clang-tidy Catan.cpp Catan.hpp $(TIDY_FLAGS) 
	# make -C game_piece tidy
	# make -C player tidy
	# make -C cards tidy

Catan.o: Catan.cpp
	$(CXX) $(CXXFLAGS) -c Catan.cpp

main.o: main.cpp
	$(CXX) $(CXXFLAGS) -c main.cpp

./tests/test.o: ./tests/test.cpp
	$(CXX) $(CXXFLAGS) -c ./tests/test.cpp -o ./tests/test.o

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $<

game_piece:
	make -C game_piece all

player:
	make -C player all

cards:
	make -C cards all


clean:
	rm -f *.o $(PROG) 
	make -C game_piece clean
	make -C player clean
	make -C cards clean
	make -C tests clean