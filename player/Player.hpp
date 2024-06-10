// @author: shay.gali@msmail.ariel.ac.il
#pragma once
#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include "../cards/Card.hpp"
#include "../game_piece/resource.hpp"

using std::vector, std::string, std::pair, std::cout, std::cin;

// Forward declaration of Catan
class Catan;

enum class PlayerColor {
    RED,
    BLUE,
    YELLOW
};

/**
 * @brief Player class
 * this class represents a real player that play the game through the terminal
 * each player has a color, victory points, resource count, development cards and knights counter

 */
class Player {
   private:
    PlayerColor color;
    int victoryPoints;
    int knights_counter;
    vector<int> resourceCount;  // WOOD, CLAY, SHEEP, WHEAT, STONE
    vector<Card*> devCards;

   public:
    Player(PlayerColor color);
    ~Player();
    Player& operator=(const Player& other);
    Player(const Player& other);

    /**
     * @brief play a turn of the player
     * Will ask the player to choose an action to do in his turn until the input is valid and he ends his turn
     */
    void play_turn(Catan& game);

    /**
     * @brief Get the color of the player (RED, BLUE, YELLOW)
     */
    string get_color() const;

    /**
     * @brief Get the color code of the player. a color code is what make the text colored in the terminal
     */
    string get_color_code() const;

    int get_victory_points() const;
    /**
     * @brief add victory points to the player
     * @param points the number of points to add
     */
    void add_victory_points(int points);

    /**
     * @brief get the number of resources of a specific type
     */
    int get_resource_count(resource resource) const;
    /**
     * @brief get the total number of resources the player has
     */
    int get_total_resources() const;
    void add_resource(resource resource, int count);
    void use_resource(resource resource, int count);
    void display_resources() const;
    void robber();

    int place_settlement(Catan& game, bool first_round = false);
    void place_road(Catan& game, bool first_round = false);
    void place_city(Catan& game);

    /**
     * @brief make a trade with the other players. will ask the user to choose the resources and development cards to offer and request
     * @param game the game object
     */
    void make_trade(Catan& game);

    /**
     * @brief get the trade offer from other player (from stdin)
     * @param trader the player that offers the trade
     * @param offer_res the resources that the trader offers
     * @param offer_dev the development cards that the trader offers
     * @param request_res the resources that the trader requests
     * @param request_dev the development cards that the trader requests
     * @return true if the player accepts the trade, false otherwise
     */
    bool trade_request(Player& trader, const vector<pair<resource, int>>& offer_res, const vector<Card*>& offer_dev, const vector<pair<resource, int>>& request_res, const vector<pair<CardType, int>>& request_dev);

    void display_dev_cards() const;

    /**
     * @brief ask the user to choose a dev card to use
     * @param game the game object (will be pass to the play_dev_card function that actually use the dev card)
     */
    void play_dev_card(Catan& game);
    /**
     * @brief use the dev card
     * @param game the game object
     * @param card the dev card to use
     *
     */
    void play_dev_card(Catan& game, Card* card);

    /**
     * @brief buy a dev card for the player
     */
    void buy_dev_card(Catan& game);

    /**
     * @brief return the first dev card of the given type
     */
    Card* get_dev_card(CardType type);

    /**
     * @brief remove the dev card from the player
     */
    Card* remove_dev_card(Card* card);

    /**
     * @brief add a dev card to the player (the card will be free in the destructor of the player object)
     */
    void add_dev_card(Card* card);

    /**
     * @brief add a 1 to the knights counter
     */
    void add_knight();

    /**
     * @brief remove a 1 from the knights counter
     */
    void remove_knight();
    int get_knights() const;

    /**
     * @brief get the number of dev cards of the given type
     */
    int get_dev_card_count(const CardType& type);

    /**
     * @brief get all the dev cards of the player
     */
    vector<Card*> get_dev_cards();

    /**
     * @brief two players are equal if they have the same color
     */
    bool operator==(const Player& other) const;

    /**
     * @brief two players are not equal if they have different colors
     */
    bool operator!=(const Player& other) const;
};