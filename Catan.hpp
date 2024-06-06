#pragma once

#include <utility>
#include <vector>

#include "cards/Card.hpp"
#include "game_piece/LandVertex.hpp"
#include "game_piece/RoadEdge.hpp"
#include "game_piece/resource.hpp"
#include "player/Player.hpp"

constexpr int NUM_PLAYERS = 3;
constexpr int POINTS_TO_WIN = 10;
class Catan {
   private:
    int current_player_index;
    vector<Card*> dev_cards;
    Player* players[3];
    LandVertex vertices[54];
    RoadEdge edges[72];

    void init_game();
    void init_vertices();
    void init_edges();
    void init_board();
    void init_dev_cards();

   public:
    ~Catan();
    Catan(Player& player1, Player& player2, Player& player3);

    LandVertex* get_vertices() { return vertices; }
    RoadEdge* get_edges() { return edges; }
    Player** get_players();
    void first_round();
    void play_turn();
    Player* start_game();

    void display_board();
    Player* is_game_over();

    void place_settlement(int vertex_id, Player& player, bool first_round = false);
    void place_road(int edge_id, Player& player, bool first_round = false);
    void place_city(int vertex_id, Player& player);
    void play_dev_card(Player& player, Card& card);

    void roll_dice();
    void give_resources(int dices_sum);
    void return_resources_on_seven_roll();

    Card* buy_dev_card(Player& player);
    void use_dev_card(Player& player, Card* card);

    void trade(Player& trader, const vector<pair<resource, int>>& offer_res, const vector<pair<Card*, int>>& offer_dev, const vector<pair<resource, int>>& request_res, const vector<pair<Card*, int>>& request_dev);
};