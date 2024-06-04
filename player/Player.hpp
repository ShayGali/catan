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
    int resourceCount[5];  // WOOD, CLAY, SHEEP, WHEAT, STONE
    int victoryPoints;
    vector<Card*> devCards;
    int knights_counter;

   public:
    Player(PlayerColor color);

    void play_turn(Catan& catan);

    string get_color();
    string get_color_code();

    string get_player_info();

    int get_victory_points();
    void add_victory_points(int points);

    int get_resource_count(resource resource);
    void add_resource(resource resource, int count);
    void use_resource(resource resource, int count);
    void display_resources();

    void display_dev_cards();
    void use_dev_card(int card_index);

    void place_settlement(Catan& game);
    void place_road(Catan& game);
    void place_city(Catan& game);

    bool trade_request(Player& trader, const vector<pair<resource, int>>& offer_res, const vector<pair<Card*, int>>& offer_dev, const vector<pair<resource, int>>& request_res, const vector<pair<Card*, int>>& request_dev);
    void make_trade(Catan& game);

    void buy_dev_card(Catan& catan);
    void remove_dev_card(Card* card);
    void add_dev_card(Card* card);
    void add_knight();
    int get_knights();
    int get_dev_card_count(const CardType& type);

    bool operator==(const Player& player) const;
};