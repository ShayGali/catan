#pragma once

#include <utility>
#include <vector>

#include "cards/Card.hpp"
#include "game_piece/LandVertex.hpp"
#include "game_piece/RoadEdge.hpp"
#include "game_piece/resource.hpp"
#include "player/Player.hpp"

constexpr int NUM_PLAYERS = 3;
class Catan {
   private:
    RoadEdge edges[72];
    Player players[3];
    vector<Card*> dev_cards;

    int current_player_index;

    void init_game();
    void init_vertices();
    void init_edges();
    void init_board();
    void init_dev_cards();

   public:
    ~Catan();
    LandVertex vertices[54];
    Catan(Player& player1, Player& player2, Player& player3);
    void first_round();
    void play_turn();
    void print_cell_status(int cell_id);
    void display_board();
    Player* is_game_over();

    void place_settlement(int vertex_id, Player& player, bool first_round = false);
    void place_road(int edge_id, Player& player, bool first_round = false);
    void place_city(int vertex_id, Player& player);
    void play_dev_card(Player& player, Card& card);

    void roll_dice();

    Card* get_dev_card(Player& player);

    void trade(Player& trader, const vector<pair<resource, int>>& offer_res, const vector<pair<Card*, int>>& offer_dev, const vector<pair<resource, int>>& request_res, const vector<pair<Card*, int>>& request_dev);
};