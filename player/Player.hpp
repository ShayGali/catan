#pragma once
#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include "../cards/Card.hpp"
#include "../game_piece/resource.hpp"

using std::vector, std::string, std::pair, std::cout;

// Forward declaration of Catan
class Catan;

enum class PlayerColor {
    RED,
    BLUE,
    YELLOW
};

class Player {
   private:
    PlayerColor color;
    int victoryPoints;
    int knights_counter;
    int resourceCount[5];  // WOOD, CLAY, SHEEP, WHEAT, STONE
    vector<Card*> devCards;

   public:
    Player(PlayerColor color);
    ~Player();
    void play_turn(Catan& catan);

    string get_color();
    string get_color_code();

    string get_player_info();

    int get_victory_points();
    void add_victory_points(int points);

    int get_resource_count(resource resource);
    int get_total_resources();
    void add_resource(resource resource, int count);
    void use_resource(resource resource, int count);
    void display_resources();
    void return_resources_on_seven_roll();

    void display_dev_cards();
    void use_dev_card(Catan& game);

    int place_settlement(Catan& game, bool first_round = false);
    void place_road(Catan& game, bool first_round = false);
    void place_city(Catan& game);

    bool trade_request(Player& trader, const vector<pair<resource, int>>& offer_res, const vector<pair<Card*, int>>& offer_dev, const vector<pair<resource, int>>& request_res, const vector<pair<Card*, int>>& request_dev);
    void make_trade(Catan& game);

    void buy_dev_card(Catan& catan);
    void remove_dev_card(Card* card);
    void add_dev_card(Card* card);
    void add_knight();
    int get_knights();
    int get_dev_card_count(const CardType& type);

    bool operator==(const Player& other) const;
    bool operator!=(const Player& other) const;
};