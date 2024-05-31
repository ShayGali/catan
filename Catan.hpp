#pragma once
#include "game_piece/LandVertex.hpp"
#include "game_piece/RoadEdge.hpp"

class Catan {
   private:
    LandVertex vertices[54];
    RoadEdge edges[72];
    Player players[3];

    void init_game();
    void init_vertices();
    void init_edges();
    void init_board();

   public:
    Catan(Player player1, Player player2, Player player3);

    void print_cell_status(int cell_id);
};