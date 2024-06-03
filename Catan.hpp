#pragma once

#include "cards/Card.hpp"
#include "game_piece/LandVertex.hpp"
#include "game_piece/RoadEdge.hpp"
#include "player/Player.hpp"

class Catan {
   private:
    LandVertex vertices[54];
    RoadEdge edges[72];
    Player players[3];
    vector<Card*> dev_cards;

    int current_player_index;

    void init_game();
    void init_vertices();
    void init_edges();
    void init_board();

   public:
    Catan(Player& player1, Player& player2, Player& player3);

    void play();
    void print_cell_status(int cell_id);

    Player* is_game_over();

    void place_settlement(int vertex_id, Player& player, bool need_resources = true);
    void place_road(int edge_id, Player& player, bool need_resources = true);
    void place_city(int vertex_id, Player& player);
    void play_dev_card(Player& player, Card& card);

    void roll_dice();

    Card* get_dev_card(Player& player);
};