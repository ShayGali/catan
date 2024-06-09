#pragma once

#include <utility>
#include <vector>

#include "cards/Card.hpp"
#include "game_piece/LandVertex.hpp"
#include "game_piece/RoadEdge.hpp"
#include "game_piece/resource.hpp"
#include "player/Player.hpp"

// constants
constexpr int NUM_PLAYERS = 3;
constexpr int POINTS_TO_WIN = 10;

/**
 * @brief The main class of the game
 * it hold the game board, the players and the dev cards, and responsible for the game logic/
 * 
 * the board is represented by a graph, with vertices and edges.
 */
class Catan {
   private:
    int current_player_index;
    vector<Player*> players;
    vector<LandVertex> vertices;
    vector<RoadEdge> edges;
    vector<Card*> dev_cards;

    /**
     * @brief Initialize the vertices of the board
     * each board knows its neighbors vertices and edges
     */
    void init_vertices();
    /**
     * @brief Initialize the edges of the board
     * each edge knows its neighbors vertices and edges
     */
    void init_edges();

    /**
     * @brief Initialize the board - the resources and the numbers on the vertices
     * for now the board is the same as the original game for each game for simplicity
     */
    void init_board();

    /**
     * @brief Initialize the dev cards
     * */
    void init_dev_cards();

   public:
    Catan(Player& player1, Player& player2, Player& player3);

    // destructor, copy constructor and assignment operator

    Catan(const Catan& other);
    ~Catan();
    Catan& operator=(const Catan& other);

    // getters

    vector<LandVertex> get_vertices() { return vertices; }
    vector<RoadEdge> get_edges() { return edges; }
    vector<Player*> get_players() { return players; }
    vector<Card*> get_dev_cards() { return dev_cards; }

    /**
     * @brief the first round of the game. each player places 2 settlements and 2 roads
     */
    void first_round();
    /**
     * @brief after the first round, the game continues with the regular turns.
     */
    void play_turn();

    /**
     * @brief start the game - call first_round and then play_turn until the game is over.
     * @return the winner of the game
     */
    Player* start_game();

    /**
     * @brief print to the stdout the board
     */
    void display_board();

    /**
     * @brief check if the game is over
     * @return the winner of the game, or nullptr if the game is not over
     */
    Player* is_game_over();

    /**
     * @brief place a settlement on the board in the given vertex
     * @param vertex_id the id of the vertex to place the settlement
     * @param player the player that places the settlement
     * @param first_round if it is the first round of the game - the player will not pay for the settlement, and he can place it any where except near other settlements
     * @throw invalid_argument if the vertex_id is not valid, the vertex is already occupied, or the player does not have enough resources, there is a settlement near the vertex, there is not a road near the vertex(not in the first round)
     */
    void place_settlement(int vertex_id, Player& player, bool first_round = false);

    /**
     * @brief place a road on the board in the given edge
     * @param edge_id the id of the edge to place the road
     * @param player the player that places the road
     * @param first_round if it is the first round of the game - the player will not pay for the road, and he can place it any where except near other roads
     * @throw invalid_argument if the edge_id is not valid, the edge is already occupied, or the player does not have enough resources, there is not a settlement or edge near the edge
     */
    void place_road(int edge_id, Player& player, bool first_round = false);

    /**
     * @brief place a city on the board in the given vertex
     * if the card is VP card, the player will win the game
     * if the card is knight card, the player knight card counter will be increased. when the player has 3 knight cards, he will get the largest army card (2 VP)
     * @param vertex_id the id of the vertex to place the city
     * @param player the player that places the city
     * @throw invalid_argument if the vertex_id is not valid, the vertex is not occupied by the player, the player does not have enough resources, the vertex is already a city
     * @throw runtime_error if there is no cards left
     * 
     */
    void place_city(int vertex_id, Player& player);

    /**
     * @brief roll the dice and give the resources to the players
     * if the sum of the dices is 7, the player with more than 7 cards will lose half of his cards
     */
    void roll_dice();

    /**
     * helper functions to give resources to the players on the roll of the dice
     * @param dices_sum the sum of the dices
     */
    void give_resources(int dices_sum);

    /**
     * helper functions to take resources from the player with more than 7 cards on 7 roll.
     */
    void return_resources_on_seven_roll();

    /**
     * @brief buy a dev card for the player
     * @param player the player that buys the dev card
     * @return the dev card that the player bought
     * @throw invalid_argument if the player does not have enough resources
     */
    Card* buy_dev_card(Player& player);

    /**
     * @brief use a dev card for the player
     * @param player the player that uses the dev card
     * @param card the dev card to use
     * @throw invalid_argument if the card is not Promotion card
     */
    void use_dev_card(Player& player, Card* card);

    void make_trade_offer(Player& trader, const vector<pair<resource, int>>& offer_res, const vector<Card*>& offer_dev, const vector<pair<resource, int>>& request_res, const vector<pair<CardType, int>>& request_dev);
};