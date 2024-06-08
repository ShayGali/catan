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
    Player& operator=(const Player& other);
    Player(const Player& other);

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

    int place_settlement(Catan& game, bool first_round = false);
    void place_road(Catan& game, bool first_round = false);
    void place_city(Catan& game);

    bool trade_request(Player& trader, const vector<pair<resource, int>>& offer_res, const vector<Card*>& offer_dev, const vector<pair<resource, int>>& request_res, const vector<pair<CardType, int>>& request_dev);
    void make_trade(Catan& game);

    void display_dev_cards();
    void use_dev_card(Catan& game);
    void use_dev_card(Catan& game, Card* card);
    void buy_dev_card(Catan& catan);
    Card* get_dev_card(CardType type);
    Card* remove_dev_card(Card* card);
    void add_dev_card(Card* card);
    void add_knight();
    void remove_knight();
    int get_knights();
    int get_dev_card_count(const CardType& type);
    vector<Card*> get_dev_cards();

    bool operator==(const Player& other) const;
    bool operator!=(const Player& other) const;
};