#pragma once
#include <string>
#include <vector>

#include "../cards/Card.hpp"
#include "../game_piece/resource.hpp"

using std::vector, std::string;

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

    string get_player_info();

    int get_victory_points();
    void add_victory_points(int points);

    int get_resource_count(resource resource);
    void add_resource(resource resource, int count);
    void use_resource(resource resource, int count);
    void display_resources();

    void buy_dev_card(Catan& catan);

    void display_dev_cards();
    void use_dev_card(int card_index);

    void place_settlement(Catan& game);
    void place_road(Catan& game);
    void place_city(Catan& game);
    void make_trade(Catan& game);

    void add_knight();
    int get_knights();
    
};