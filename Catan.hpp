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
    vector<Player*> players;
    vector<LandVertex> vertices;
    vector<RoadEdge> edges;
    vector<Card*> dev_cards;

    void init_vertices();
    void init_edges();
    void init_board();
    void init_dev_cards();

   public:
    Catan(Player& player1, Player& player2, Player& player3);
    Catan(const Catan& other);
    ~Catan();
    Catan& operator=(const Catan& other);

    vector<LandVertex> get_vertices() { return vertices; }
    vector<RoadEdge> get_edges() { return edges; }
    vector<Player*> get_players() { return players; }

    void first_round();
    void play_turn();
    Player* start_game();

    void display_board();
    Player* is_game_over();

    void place_settlement(int vertex_id, Player& player, bool first_round = false);
    void place_road(int edge_id, Player& player, bool first_round = false);
    void place_city(int vertex_id, Player& player);

    void roll_dice();
    void give_resources(int dices_sum);
    void return_resources_on_seven_roll();

    Card* buy_dev_card(Player& player);
    void use_dev_card(Player& player, Card* card);

    void make_trade_offer(Player& trader, const vector<pair<resource, int>>& offer_res, const vector<Card*>& offer_dev, const vector<pair<resource, int>>& request_res, const vector<pair<CardType, int>>& request_dev);
};