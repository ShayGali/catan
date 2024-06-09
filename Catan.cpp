#include "Catan.hpp"

#include <ctime>
#include <iostream>
#include <stdexcept>

#include "cards/KngihtCard.hpp"
#include "cards/MonopolyCard.hpp"
#include "cards/RoadBuildCard.hpp"
#include "cards/VictoryPointCard.hpp"
#include "cards/YearOfPlentyCard.hpp"

Catan::Catan(Player& player1, Player& player2, Player& player3)
    : players{&player1, &player2, &player3}, vertices(54), edges(72) {
    // initialize the random seed
    // srand(time(nullptr));
    srand(1);
    current_player_index = 0;

    init_vertices();
    init_edges();
    init_board();
    init_dev_cards();
}

Catan::Catan(const Catan& other) {
    current_player_index = other.current_player_index;
    for (int i = 0; i < 54; i++) {
        vertices[i] = other.vertices[i];
    }
    for (int i = 0; i < 72; i++) {
        edges[i] = other.edges[i];
    }
    for (int i = 0; i < NUM_PLAYERS; i++) {
        players[i] = other.players[i];
    }
    for (int i = 0; i < other.dev_cards.size(); i++) {
        dev_cards.push_back(other.dev_cards[i]->clone());
    }
}

Catan::~Catan() {
    for (int i = 0; i < dev_cards.size(); i++) {
        delete dev_cards[i];
    }
}

Catan& Catan::operator=(const Catan& other) {
    if (this == &other) {
        return *this;
    }

    // delete old dev cards
    for (int i = 0; i < dev_cards.size(); i++) {
        delete dev_cards[i];
    }
    dev_cards.clear();

    // copy new dev cards
    for (int i = 0; i < other.dev_cards.size(); i++) {
        dev_cards.push_back(other.dev_cards[i]->clone());
    }

    current_player_index = other.current_player_index;
    for (int i = 0; i < 54; i++) {
        vertices[i] = other.vertices[i];
    }
    for (int i = 0; i < 72; i++) {
        edges[i] = other.edges[i];
    }
    for (int i = 0; i < NUM_PLAYERS; i++) {
        players[i] = other.players[i];
    }

    return *this;
}

void Catan::first_round() {
    display_board();
    for (int i = 0; i < NUM_PLAYERS; i++) {
        cout << players[i]->get_color() << " turn\n";
        players[i]->place_settlement(*this, true);
        display_board();
        players[i]->place_road(*this, true);
        display_board();
    }
    for (int i = NUM_PLAYERS - 1; i >= 0; i--) {
        if (i != NUM_PLAYERS - 1) {
            cout << players[i]->get_color() << " turn\n";
        }
        int vertex_id = players[i]->place_settlement(*this, true);
        LandVertex& vertex = vertices[vertex_id];
        for (int j = 0; j < 3; j++) {
            players[i]->add_resource(vertex.get_resources()[j].first, 1);
        }
        display_board();
        players[i]->place_road(*this, true);
        display_board();
    }
}

void Catan::play_turn() {
    display_board();
    Player* current_player = players[current_player_index];
    cout << current_player->get_color() << " turn\n";
    current_player->play_turn(*this);
    current_player_index = (current_player_index + 1) % NUM_PLAYERS;
}

Player* Catan::is_game_over() {
    if (players[0]->get_victory_points() >= POINTS_TO_WIN) {
        return players[0];
    }
    if (players[1]->get_victory_points() >= POINTS_TO_WIN) {
        return players[1];
    }
    if (players[2]->get_victory_points() >= POINTS_TO_WIN) {
        return players[2];
    }
    return nullptr;
}

void Catan::place_settlement(int vertex_id, Player& player, bool first_round) {
    // check if the vertex_id is valid
    if (vertex_id < 0 || vertex_id > 53) {
        throw std::invalid_argument("Invalid vertex id - must be between 0 and 53! (got " + std::to_string(vertex_id) + ")");
    }

    LandVertex& vertex = vertices[vertex_id];
    // check if the vertex is empty
    if (vertex.get_owner() != nullptr) {
        throw std::invalid_argument("Vertex is already occupied! (owner: " + vertex.get_owner()->get_color() + ")");
    }

    if (!first_round) {
        // check if there is a road connected to the vertex
        for (int i = 0; i < 3; i++) {
            if (vertex.get_adjacent_edge(i) != nullptr) {
                if (vertex.get_adjacent_edge(i)->get_owner() == &player) {
                    break;
                }
            }
            if (i == 2) {
                throw std::invalid_argument("No road connected to the vertex!");
            }
        }
    }
    // check if there is a settlement nearby
    for (int i = 0; i < 3; i++) {
        if (vertex.get_adjacent_vertex(i) != nullptr) {
            if (vertex.get_adjacent_vertex(i)->get_owner() != nullptr) {
                throw std::invalid_argument("There is a settlement nearby! (trying to place a settlement at vertex " + std::to_string(vertex_id) + " and there is a settlement at vertex " + std::to_string(vertex.get_adjacent_vertex(i)->get_id()) + ")");
            }
        }
    }

    if (!first_round) {
        // check if the player has enough resources
        if (player.get_resource_count(resource::WOOD) < 1 || player.get_resource_count(resource::CLAY) < 1 || player.get_resource_count(resource::SHEEP) < 1 || player.get_resource_count(resource::WHEAT) < 1) {
            throw std::invalid_argument("Not enough resources to place a settlement!");
        }

        player.use_resource(resource::WOOD, 1);
        player.use_resource(resource::CLAY, 1);
        player.use_resource(resource::SHEEP, 1);
        player.use_resource(resource::WHEAT, 1);
    }
    // place the settlement
    vertex.set_owner(&player);
    player.add_victory_points(1);
}

void Catan::place_road(int edge_id, Player& player, bool first_round) {
    // check if the edge_id is valid
    if (edge_id < 0 || edge_id > 71) {
        throw std::invalid_argument("Invalid edge id - must be between 0 and 71! (got " + std::to_string(edge_id) + ")");
    }

    RoadEdge& edge = edges[edge_id];

    // check if the edge is empty
    if (edge.get_owner() != nullptr) {
        throw std::invalid_argument("Edge is already occupied! (owner: " + edge.get_owner()->get_color() + ")");
    }

    // check if it is near a settlement/city or a road
    if (&player != edge.get_adjacent_vertex(0)->get_owner() && &player != edge.get_adjacent_vertex(1)->get_owner()) {  // check if the player have a connected settlement
        // check if the player have a connected road
        for (int i = 0; i < 4; i++) {
            if (edge.get_adjacent_edge(i) != nullptr) {
                if (edge.get_adjacent_edge(i)->get_owner() == &player) {
                    break;
                }
            }
            if (i == 3) {
                throw std::invalid_argument("No connected road/settlement nearby!");
            }
        }
    }

    if (!first_round) {
        // check if the player has enough resources
        if (player.get_resource_count(resource::WOOD) < 1 || player.get_resource_count(resource::CLAY) < 1) {
            throw std::invalid_argument("Not enough resources to place a road!");
        }

        player.use_resource(resource::WOOD, 1);
        player.use_resource(resource::CLAY, 1);
    }

    // place the road
    edge.set_owner(&player);
}

void Catan::place_city(int vertex_id, Player& player) {
    // check if the vertex_id is valid
    if (vertex_id < 0 || vertex_id > 53) {
        throw std::invalid_argument("Invalid vertex id - must be between 0 and 53! (got " + std::to_string(vertex_id) + ")");
    }

    LandVertex& vertex = vertices[vertex_id];

    // check if the vertex is the player's settlement
    if (vertex.get_owner() == nullptr || *(vertex.get_owner()) != player) {
        throw std::invalid_argument("Vertex is not owned by the player!");
    }

    if (vertex.get_isCity()) {
        throw std::invalid_argument("Vertex is already a city!");
    }

    // check if the player has enough resources
    if (player.get_resource_count(resource::WHEAT) < 2 || player.get_resource_count(resource::STONE) < 3) {
        throw std::invalid_argument("Not enough resources to place a city!");
    }

    player.use_resource(resource::WHEAT, 2);
    player.use_resource(resource::STONE, 3);

    // place the city
    vertex.upgrade_to_city();
    player.add_victory_points(1);
}

void Catan::make_trade_offer(Player& trader, const vector<pair<resource, int>>& offer_res, const vector<Card*>& offer_dev, const vector<pair<resource, int>>& request_res, const vector<pair<CardType, int>>& request_dev) {
    // check if the player has enough resources
    for (int i = 0; i < offer_res.size(); i++) {
        if (trader.get_resource_count(offer_res[i].first) < offer_res[i].second) {
            throw std::invalid_argument("Not enough resources to trade! (trying to trade " + std::to_string(offer_res[i].second) + " " + offer_res[i].first.get_emoji() + " )");
        }
    }

    // check if the player have the development cards
    for (int i = 0; i < offer_dev.size(); i++) {
        if (trader.get_dev_card_count(offer_dev[i]->type()) < 1) {
            throw std::invalid_argument("You do not have the development card to trade! (trying to trade " + Card::emoji_from_type(offer_dev[i]->type()) + "  )");
        }
    }

    // offer each player the offer
    for (int i = 0; i < NUM_PLAYERS; i++) {
        if (*players[i] == trader) {
            continue;
        }

        // ask the player if they want to trade
        if (players[i]->trade_request(trader, offer_res, offer_dev, request_res, request_dev)) {
            bool need_continue = false;
            // check if the player has enough resources
            for (int j = 0; j < request_res.size(); j++) {
                if (players[i]->get_resource_count(request_res[j].first) < request_res[j].second) {
                    cout << "Player " << players[i]->get_color() << " does not have enough resources to trade! (trying to trade " << request_res[j].second << " " << request_res[j].first.get_emoji() << " )\n";
                    need_continue = true;
                    break;
                }
            }

            if (need_continue) {
                continue;  // skip the player
            }

            // check if the player have the development cards
            for (int j = 0; j < request_dev.size(); j++) {
                if (players[i]->get_dev_card_count(request_dev[j].first) < request_dev[j].second) {
                    cout << "Player " << players[i]->get_color() << " does not have enough development cards to trade! (trying to trade " << request_dev[j].second << " " << Card::emoji_from_type(request_dev[j].first) << "  )\n";
                    need_continue = true;
                    break;
                }
            }

            if (need_continue) {
                continue;  // skip the player
            }

            // trade the resources
            for (int j = 0; j < offer_res.size(); j++) {
                trader.use_resource(offer_res[j].first, offer_res[j].second);
                players[i]->add_resource(offer_res[j].first, offer_res[j].second);
            }

            for (int j = 0; j < request_res.size(); j++) {
                players[i]->use_resource(request_res[j].first, request_res[j].second);
                trader.add_resource(request_res[j].first, request_res[j].second);
            }

            // trade the development cards
            for (int j = 0; j < offer_dev.size(); j++) {
                Card* card = trader.remove_dev_card(offer_dev[j]);
                players[i]->add_dev_card(card);
                if (card->type() == CardType::KNIGHT) {
                    trader.remove_knight();
                    if (trader.get_knights() == 2) {
                        trader.add_victory_points(-2);
                    }

                    players[i]->add_knight();
                    if (players[i]->get_knights() >= 3) {
                        players[i]->add_victory_points(2);
                    }
                } else if (card->type() == CardType::VICTORY_POINT) {
                    trader.add_victory_points(-1);
                    players[i]->add_victory_points(1);
                }
            }

            for (int j = 0; j < request_dev.size(); j++) {
                Card* card = players[i]->remove_dev_card(players[i]->get_dev_card(request_dev[j].first));
                trader.add_dev_card(card);

                if (card->type() == CardType::KNIGHT) {
                    players[i]->remove_knight();
                    if (players[i]->get_knights() == 2) {
                        players[i]->add_victory_points(-2);
                    }

                    trader.add_knight();
                    if (trader.get_knights() >= 3) {
                        trader.add_victory_points(2);
                    }
                } else if (card->type() == CardType::VICTORY_POINT) {
                    players[i]->add_victory_points(-1);
                    trader.add_victory_points(1);
                }
            }

            cout << "Trade successful! " << trader.get_color() << " traded with " << players[i]->get_color() << "\n";
            // end the trade
            return;
        }
    }
}

void Catan::roll_dice() {
    int dice_1 = rand() % 6 + 1;
    int dice_2 = rand() % 6 + 1;
    int sum = dice_1 + dice_2;

    std::cout << "Dice 1: " << dice_1
              << "\nDice 2: " << dice_2
              << "\nSum: " << sum << std::endl;
    if (sum == 7) {
        return_resources_on_seven_roll();
    } else {
        give_resources(sum);
    }
}

void Catan::give_resources(int dices_sum) {
    for (int i = 0; i < 54; i++) {
        for (int j = 0; j < 3; j++) {
            if (vertices[i].get_resources()[j].second == dices_sum && vertices[i].get_owner() != nullptr) {
                if (vertices[i].get_isCity()) {
                    cout << "Player " << vertices[i].get_owner()->get_color() << " gets 2 " << vertices[i].get_resources()[j].first.get_emoji() << " from vertex " << i << "\n";
                    vertices[i].get_owner()->add_resource(vertices[i].get_resources()[j].first, 2);
                } else {
                    cout << "Player " << vertices[i].get_owner()->get_color() << " gets 1 " << vertices[i].get_resources()[j].first.get_emoji() << " from vertex " << i << "\n";
                    vertices[i].get_owner()->add_resource(vertices[i].get_resources()[j].first, 1);
                }
            }
        }
    }
}

void Catan::return_resources_on_seven_roll() {
    for (int i = 0; i < NUM_PLAYERS; i++) {
        if (players[i]->get_total_resources() > 7) {
            players[i]->return_resources_on_seven_roll();
        }
    }
}

Card* Catan::buy_dev_card(Player& player) {
    // check if there are any cards left
    if (dev_cards.empty()) {
        throw std::runtime_error("No development cards left");
    }

    // check if player has enough resources
    if (player.get_resource_count(resource::WHEAT) < 1 || player.get_resource_count(resource::STONE) < 1 || player.get_resource_count(resource::SHEEP) < 1) {
        throw std::runtime_error("Player does not have enough resources to buy a development card");
    }

    // remove resources from player
    player.use_resource(resource::WHEAT, 1);
    player.use_resource(resource::STONE, 1);
    player.use_resource(resource::SHEEP, 1);

    // get random card
    int num_of_cards = dev_cards.size();
    int rand_index = rand() % num_of_cards;
    Card* card = dev_cards[rand_index];
    cout << (rand_index) << " " << num_of_cards << "\n";
    cout << card << "\n";


    // remove card from deck
    dev_cards.erase(dev_cards.begin() + rand_index);

    if (dynamic_cast<KnightCard*>(card) != nullptr) {
        player.add_knight();
        if (player.get_knights() >= 3) {
            player.add_victory_points(2);
        }
    }

    if (dynamic_cast<VictoryPointCard*>(card) != nullptr) {
        player.add_victory_points(1);
    }

    return card;
}

void Catan::use_dev_card(Player& player, Card* card) {
    // check if the card is promotion card
    PromotionCard* cast_card = dynamic_cast<PromotionCard*>(card);
    if (cast_card == nullptr) {
        throw std::invalid_argument("card is not PromotionCard");
    }
    cast_card->use(*this, player);
}

Player* Catan::start_game() {
    Player* winner = nullptr;
    first_round();
    while ((winner = is_game_over()) == nullptr) {
        play_turn();
    }
    return winner;
}

void Catan::init_vertices() {
    // initialize the vertices
    for (int i = 0; i < 54; i++) {
        vertices[i] = LandVertex(i);
    }

    // first vertex row
    vertices[0].set_adjacent_vertex(&vertices[3], &vertices[4], nullptr);
    vertices[1].set_adjacent_vertex(&vertices[4], &vertices[5], nullptr);
    vertices[2].set_adjacent_vertex(&vertices[5], &vertices[6], nullptr);

    vertices[0].set_adjacent_edge(&edges[0], &edges[1], nullptr);
    vertices[1].set_adjacent_edge(&edges[2], &edges[3], nullptr);
    vertices[2].set_adjacent_edge(&edges[4], &edges[5], nullptr);

    // second vertex row
    vertices[3].set_adjacent_vertex(&vertices[0], &vertices[7], nullptr);
    vertices[4].set_adjacent_vertex(&vertices[0], &vertices[1], &vertices[8]);
    vertices[5].set_adjacent_vertex(&vertices[1], &vertices[2], &vertices[9]);
    vertices[6].set_adjacent_vertex(&vertices[2], &vertices[10], nullptr);

    vertices[3].set_adjacent_edge(&edges[0], &edges[6], nullptr);
    vertices[4].set_adjacent_edge(&edges[1], &edges[2], &edges[7]);
    vertices[5].set_adjacent_edge(&edges[3], &edges[4], &edges[8]);
    vertices[6].set_adjacent_edge(&edges[5], &edges[9], nullptr);

    // third vertex row
    vertices[7].set_adjacent_vertex(&vertices[3], &vertices[11], &vertices[12]);
    vertices[8].set_adjacent_vertex(&vertices[4], &vertices[12], &vertices[13]);
    vertices[9].set_adjacent_vertex(&vertices[5], &vertices[13], &vertices[14]);
    vertices[10].set_adjacent_vertex(&vertices[6], &vertices[14], &vertices[15]);

    vertices[7].set_adjacent_edge(&edges[6], &edges[10], &edges[11]);
    vertices[8].set_adjacent_edge(&edges[7], &edges[12], &edges[13]);
    vertices[9].set_adjacent_edge(&edges[8], &edges[14], &edges[15]);
    vertices[10].set_adjacent_edge(&edges[9], &edges[16], &edges[17]);

    // fourth vertex row
    vertices[11].set_adjacent_vertex(&vertices[7], &vertices[16], nullptr);
    vertices[12].set_adjacent_vertex(&vertices[7], &vertices[8], &vertices[17]);
    vertices[13].set_adjacent_vertex(&vertices[8], &vertices[9], &vertices[18]);
    vertices[14].set_adjacent_vertex(&vertices[9], &vertices[10], &vertices[19]);
    vertices[15].set_adjacent_vertex(&vertices[10], &vertices[20], nullptr);

    vertices[11].set_adjacent_edge(&edges[10], &edges[18], nullptr);
    vertices[12].set_adjacent_edge(&edges[11], &edges[12], &edges[19]);
    vertices[13].set_adjacent_edge(&edges[13], &edges[14], &edges[20]);
    vertices[14].set_adjacent_edge(&edges[15], &edges[16], &edges[21]);
    vertices[15].set_adjacent_edge(&edges[17], &edges[22], nullptr);

    // fifth vertex row
    vertices[16].set_adjacent_vertex(&vertices[11], &vertices[21], &vertices[22]);
    vertices[17].set_adjacent_vertex(&vertices[12], &vertices[22], &vertices[23]);
    vertices[18].set_adjacent_vertex(&vertices[13], &vertices[23], &vertices[24]);
    vertices[19].set_adjacent_vertex(&vertices[14], &vertices[24], &vertices[25]);
    vertices[20].set_adjacent_vertex(&vertices[15], &vertices[25], &vertices[26]);

    vertices[16].set_adjacent_edge(&edges[18], &edges[23], &edges[24]);
    vertices[17].set_adjacent_edge(&edges[19], &edges[25], &edges[26]);
    vertices[18].set_adjacent_edge(&edges[20], &edges[27], &edges[28]);
    vertices[19].set_adjacent_edge(&edges[21], &edges[29], &edges[30]);
    vertices[20].set_adjacent_edge(&edges[22], &edges[31], &edges[32]);

    // sixth vertex row
    vertices[21].set_adjacent_vertex(&vertices[16], &vertices[27], nullptr);
    vertices[22].set_adjacent_vertex(&vertices[16], &vertices[17], &vertices[28]);
    vertices[23].set_adjacent_vertex(&vertices[17], &vertices[18], &vertices[29]);
    vertices[24].set_adjacent_vertex(&vertices[18], &vertices[19], &vertices[30]);
    vertices[25].set_adjacent_vertex(&vertices[19], &vertices[20], &vertices[31]);
    vertices[26].set_adjacent_vertex(&vertices[20], &vertices[32], nullptr);

    vertices[21].set_adjacent_edge(&edges[23], &edges[33], nullptr);
    vertices[22].set_adjacent_edge(&edges[24], &edges[25], &edges[34]);
    vertices[23].set_adjacent_edge(&edges[26], &edges[27], &edges[35]);
    vertices[24].set_adjacent_edge(&edges[28], &edges[29], &edges[36]);
    vertices[25].set_adjacent_edge(&edges[30], &edges[31], &edges[37]);
    vertices[26].set_adjacent_edge(&edges[32], &edges[38], nullptr);

    // seventh vertex row
    vertices[27].set_adjacent_vertex(&vertices[21], &vertices[33], nullptr);
    vertices[28].set_adjacent_vertex(&vertices[22], &vertices[33], &vertices[34]);
    vertices[29].set_adjacent_vertex(&vertices[23], &vertices[34], &vertices[35]);
    vertices[30].set_adjacent_vertex(&vertices[24], &vertices[35], &vertices[36]);
    vertices[31].set_adjacent_vertex(&vertices[25], &vertices[36], &vertices[37]);
    vertices[32].set_adjacent_vertex(&vertices[26], &vertices[37], nullptr);

    vertices[27].set_adjacent_edge(&edges[33], &edges[39], nullptr);
    vertices[28].set_adjacent_edge(&edges[34], &edges[40], &edges[41]);
    vertices[29].set_adjacent_edge(&edges[35], &edges[42], &edges[43]);
    vertices[30].set_adjacent_edge(&edges[36], &edges[44], &edges[45]);
    vertices[31].set_adjacent_edge(&edges[37], &edges[46], &edges[47]);
    vertices[32].set_adjacent_edge(&edges[38], &edges[48], nullptr);

    // eighth vertex row
    vertices[33].set_adjacent_vertex(&vertices[27], &vertices[28], &vertices[38]);
    vertices[34].set_adjacent_vertex(&vertices[28], &vertices[29], &vertices[39]);
    vertices[35].set_adjacent_vertex(&vertices[29], &vertices[30], &vertices[40]);
    vertices[36].set_adjacent_vertex(&vertices[30], &vertices[31], &vertices[41]);
    vertices[37].set_adjacent_vertex(&vertices[31], &vertices[32], &vertices[42]);

    vertices[33].set_adjacent_edge(&edges[39], &edges[40], &edges[49]);
    vertices[34].set_adjacent_edge(&edges[41], &edges[42], &edges[50]);
    vertices[35].set_adjacent_edge(&edges[43], &edges[44], &edges[51]);
    vertices[36].set_adjacent_edge(&edges[45], &edges[46], &edges[52]);
    vertices[37].set_adjacent_edge(&edges[47], &edges[48], &edges[53]);

    // ninth vertex row
    vertices[38].set_adjacent_vertex(&vertices[33], &vertices[43], nullptr);
    vertices[39].set_adjacent_vertex(&vertices[34], &vertices[43], &vertices[44]);
    vertices[40].set_adjacent_vertex(&vertices[35], &vertices[44], &vertices[45]);
    vertices[41].set_adjacent_vertex(&vertices[36], &vertices[45], &vertices[46]);
    vertices[42].set_adjacent_vertex(&vertices[37], &vertices[46], nullptr);

    vertices[38].set_adjacent_edge(&edges[49], &edges[54], nullptr);
    vertices[39].set_adjacent_edge(&edges[50], &edges[55], &edges[56]);
    vertices[40].set_adjacent_edge(&edges[51], &edges[57], &edges[58]);
    vertices[41].set_adjacent_edge(&edges[52], &edges[59], &edges[60]);
    vertices[42].set_adjacent_edge(&edges[53], &edges[61], nullptr);

    // tenth vertex row
    vertices[43].set_adjacent_vertex(&vertices[38], &vertices[39], &vertices[47]);
    vertices[44].set_adjacent_vertex(&vertices[39], &vertices[40], &vertices[48]);
    vertices[45].set_adjacent_vertex(&vertices[40], &vertices[41], &vertices[49]);
    vertices[46].set_adjacent_vertex(&vertices[41], &vertices[42], &vertices[50]);

    vertices[43].set_adjacent_edge(&edges[54], &edges[55], &edges[62]);
    vertices[44].set_adjacent_edge(&edges[56], &edges[57], &edges[63]);
    vertices[45].set_adjacent_edge(&edges[58], &edges[59], &edges[64]);
    vertices[46].set_adjacent_edge(&edges[60], &edges[61], &edges[65]);

    // eleventh vertex row
    vertices[47].set_adjacent_vertex(&vertices[43], &vertices[51], nullptr);
    vertices[48].set_adjacent_vertex(&vertices[44], &vertices[51], &vertices[52]);
    vertices[49].set_adjacent_vertex(&vertices[45], &vertices[52], &vertices[53]);
    vertices[50].set_adjacent_vertex(&vertices[46], &vertices[53], nullptr);

    vertices[47].set_adjacent_edge(&edges[62], &edges[66], nullptr);
    vertices[48].set_adjacent_edge(&edges[63], &edges[67], &edges[68]);
    vertices[49].set_adjacent_edge(&edges[64], &edges[69], &edges[70]);
    vertices[50].set_adjacent_edge(&edges[65], &edges[71], nullptr);

    // twelfth vertex row
    vertices[51].set_adjacent_vertex(&vertices[47], &vertices[48], nullptr);
    vertices[52].set_adjacent_vertex(&vertices[48], &vertices[49], nullptr);
    vertices[53].set_adjacent_vertex(&vertices[49], &vertices[50], nullptr);

    vertices[51].set_adjacent_edge(&edges[66], &edges[67], nullptr);
    vertices[52].set_adjacent_edge(&edges[68], &edges[69], nullptr);
    vertices[53].set_adjacent_edge(&edges[70], &edges[71], nullptr);
}

void Catan::init_edges() {
    // initialize the edges
    for (int i = 0; i < 72; i++) {
        edges[i] = RoadEdge(i);
    }

    // first edge row
    edges[0].set_adjacent_edge(&edges[1], &edges[6], nullptr, nullptr);
    edges[1].set_adjacent_edge(&edges[0], &edges[2], &edges[7], nullptr);
    edges[2].set_adjacent_edge(&edges[1], &edges[7], &edges[3], nullptr);
    edges[3].set_adjacent_edge(&edges[2], &edges[8], &edges[4], nullptr);
    edges[4].set_adjacent_edge(&edges[3], &edges[8], &edges[5], nullptr);
    edges[5].set_adjacent_edge(&edges[4], &edges[9], nullptr, nullptr);

    edges[0].set_adjacent_vertex(&vertices[0], &vertices[3]);
    edges[1].set_adjacent_vertex(&vertices[0], &vertices[4]);
    edges[2].set_adjacent_vertex(&vertices[1], &vertices[4]);
    edges[3].set_adjacent_vertex(&vertices[1], &vertices[5]);
    edges[4].set_adjacent_vertex(&vertices[2], &vertices[5]);
    edges[5].set_adjacent_vertex(&vertices[2], &vertices[6]);

    // second edge row
    edges[6].set_adjacent_edge(&edges[0], &edges[10], &edges[11], nullptr);
    edges[7].set_adjacent_edge(&edges[1], &edges[2], &edges[12], &edges[13]);
    edges[8].set_adjacent_edge(&edges[3], &edges[4], &edges[14], &edges[15]);
    edges[9].set_adjacent_edge(&edges[5], &edges[16], &edges[17], nullptr);

    edges[6].set_adjacent_vertex(&vertices[3], &vertices[7]);
    edges[7].set_adjacent_vertex(&vertices[4], &vertices[8]);
    edges[8].set_adjacent_vertex(&vertices[5], &vertices[9]);
    edges[9].set_adjacent_vertex(&vertices[6], &vertices[10]);

    // third edge row
    edges[10].set_adjacent_edge(&edges[6], &edges[11], &edges[18], nullptr);
    edges[11].set_adjacent_edge(&edges[6], &edges[10], &edges[12], &edges[19]);
    edges[12].set_adjacent_edge(&edges[7], &edges[11], &edges[13], &edges[19]);
    edges[13].set_adjacent_edge(&edges[7], &edges[12], &edges[14], &edges[20]);
    edges[14].set_adjacent_edge(&edges[8], &edges[13], &edges[15], &edges[20]);
    edges[15].set_adjacent_edge(&edges[8], &edges[14], &edges[16], &edges[21]);
    edges[16].set_adjacent_edge(&edges[9], &edges[15], &edges[17], &edges[21]);
    edges[17].set_adjacent_edge(&edges[9], &edges[16], &edges[22], nullptr);

    edges[10].set_adjacent_vertex(&vertices[7], &vertices[11]);
    edges[11].set_adjacent_vertex(&vertices[7], &vertices[12]);
    edges[12].set_adjacent_vertex(&vertices[8], &vertices[12]);
    edges[13].set_adjacent_vertex(&vertices[8], &vertices[13]);
    edges[14].set_adjacent_vertex(&vertices[9], &vertices[13]);
    edges[15].set_adjacent_vertex(&vertices[9], &vertices[14]);
    edges[16].set_adjacent_vertex(&vertices[10], &vertices[14]);
    edges[17].set_adjacent_vertex(&vertices[10], &vertices[15]);

    // fourth edge row
    edges[18].set_adjacent_edge(&edges[10], &edges[23], &edges[24], nullptr);
    edges[19].set_adjacent_edge(&edges[11], &edges[12], &edges[25], &edges[26]);
    edges[20].set_adjacent_edge(&edges[13], &edges[14], &edges[27], &edges[28]);
    edges[21].set_adjacent_edge(&edges[15], &edges[16], &edges[29], &edges[30]);
    edges[22].set_adjacent_edge(&edges[17], &edges[31], &edges[32], nullptr);

    edges[18].set_adjacent_vertex(&vertices[11], &vertices[16]);
    edges[19].set_adjacent_vertex(&vertices[12], &vertices[17]);
    edges[20].set_adjacent_vertex(&vertices[13], &vertices[18]);
    edges[21].set_adjacent_vertex(&vertices[14], &vertices[19]);
    edges[22].set_adjacent_vertex(&vertices[15], &vertices[20]);

    // fifth edge row
    edges[23].set_adjacent_edge(&edges[18], &edges[24], &edges[33], nullptr);
    edges[24].set_adjacent_edge(&edges[18], &edges[23], &edges[25], &edges[34]);
    edges[25].set_adjacent_edge(&edges[19], &edges[24], &edges[26], &edges[34]);
    edges[26].set_adjacent_edge(&edges[19], &edges[25], &edges[27], &edges[35]);
    edges[27].set_adjacent_edge(&edges[20], &edges[26], &edges[28], &edges[35]);
    edges[28].set_adjacent_edge(&edges[20], &edges[27], &edges[29], &edges[36]);
    edges[29].set_adjacent_edge(&edges[21], &edges[28], &edges[30], &edges[36]);
    edges[30].set_adjacent_edge(&edges[21], &edges[29], &edges[31], &edges[37]);
    edges[31].set_adjacent_edge(&edges[22], &edges[30], &edges[32], &edges[37]);
    edges[32].set_adjacent_edge(&edges[22], &edges[31], &edges[38], nullptr);

    edges[23].set_adjacent_vertex(&vertices[16], &vertices[21]);
    edges[24].set_adjacent_vertex(&vertices[16], &vertices[22]);
    edges[25].set_adjacent_vertex(&vertices[17], &vertices[22]);
    edges[26].set_adjacent_vertex(&vertices[17], &vertices[23]);
    edges[27].set_adjacent_vertex(&vertices[18], &vertices[23]);
    edges[28].set_adjacent_vertex(&vertices[18], &vertices[24]);
    edges[29].set_adjacent_vertex(&vertices[19], &vertices[24]);
    edges[30].set_adjacent_vertex(&vertices[19], &vertices[25]);
    edges[31].set_adjacent_vertex(&vertices[20], &vertices[25]);
    edges[32].set_adjacent_vertex(&vertices[20], &vertices[26]);

    // sixth edge row
    edges[33].set_adjacent_edge(&edges[23], &edges[39], nullptr, nullptr);
    edges[34].set_adjacent_edge(&edges[24], &edges[25], &edges[40], &edges[41]);
    edges[35].set_adjacent_edge(&edges[26], &edges[27], &edges[42], &edges[43]);
    edges[36].set_adjacent_edge(&edges[28], &edges[29], &edges[44], &edges[45]);
    edges[37].set_adjacent_edge(&edges[30], &edges[31], &edges[46], &edges[47]);
    edges[38].set_adjacent_edge(&edges[32], &edges[48], nullptr, nullptr);

    edges[33].set_adjacent_vertex(&vertices[21], &vertices[27]);
    edges[34].set_adjacent_vertex(&vertices[22], &vertices[28]);
    edges[35].set_adjacent_vertex(&vertices[23], &vertices[29]);
    edges[36].set_adjacent_vertex(&vertices[24], &vertices[30]);
    edges[37].set_adjacent_vertex(&vertices[25], &vertices[31]);
    edges[38].set_adjacent_vertex(&vertices[26], &vertices[32]);

    // seventh edge row
    edges[39].set_adjacent_edge(&edges[33], &edges[40], &edges[49], nullptr);
    edges[40].set_adjacent_edge(&edges[34], &edges[39], &edges[41], &edges[49]);
    edges[41].set_adjacent_edge(&edges[34], &edges[40], &edges[42], &edges[50]);
    edges[42].set_adjacent_edge(&edges[35], &edges[41], &edges[43], &edges[50]);
    edges[43].set_adjacent_edge(&edges[35], &edges[42], &edges[44], &edges[51]);
    edges[44].set_adjacent_edge(&edges[36], &edges[43], &edges[45], &edges[51]);
    edges[45].set_adjacent_edge(&edges[36], &edges[44], &edges[46], &edges[52]);
    edges[46].set_adjacent_edge(&edges[37], &edges[45], &edges[47], &edges[52]);
    edges[47].set_adjacent_edge(&edges[37], &edges[46], &edges[48], &edges[53]);
    edges[48].set_adjacent_edge(&edges[38], &edges[47], &edges[53], nullptr);

    edges[39].set_adjacent_vertex(&vertices[27], &vertices[33]);
    edges[40].set_adjacent_vertex(&vertices[28], &vertices[33]);
    edges[41].set_adjacent_vertex(&vertices[28], &vertices[34]);
    edges[42].set_adjacent_vertex(&vertices[29], &vertices[34]);
    edges[43].set_adjacent_vertex(&vertices[29], &vertices[35]);
    edges[44].set_adjacent_vertex(&vertices[30], &vertices[35]);
    edges[45].set_adjacent_vertex(&vertices[30], &vertices[36]);
    edges[46].set_adjacent_vertex(&vertices[31], &vertices[36]);
    edges[47].set_adjacent_vertex(&vertices[31], &vertices[37]);
    edges[48].set_adjacent_vertex(&vertices[32], &vertices[37]);

    // eighth edge row
    edges[49].set_adjacent_edge(&edges[39], &edges[50], &edges[54], nullptr);
    edges[50].set_adjacent_edge(&edges[41], &edges[42], &edges[55], &edges[56]);
    edges[51].set_adjacent_edge(&edges[43], &edges[44], &edges[57], &edges[58]);
    edges[52].set_adjacent_edge(&edges[45], &edges[46], &edges[59], &edges[60]);
    edges[53].set_adjacent_edge(&edges[47], &edges[48], &edges[61], nullptr);

    edges[49].set_adjacent_vertex(&vertices[33], &vertices[38]);
    edges[50].set_adjacent_vertex(&vertices[34], &vertices[39]);
    edges[51].set_adjacent_vertex(&vertices[35], &vertices[40]);
    edges[52].set_adjacent_vertex(&vertices[36], &vertices[41]);
    edges[53].set_adjacent_vertex(&vertices[37], &vertices[42]);

    // ninth edge row
    edges[54].set_adjacent_edge(&edges[49], &edges[55], &edges[62], nullptr);
    edges[55].set_adjacent_edge(&edges[50], &edges[54], &edges[56], &edges[62]);
    edges[56].set_adjacent_edge(&edges[50], &edges[55], &edges[57], &edges[63]);
    edges[57].set_adjacent_edge(&edges[51], &edges[56], &edges[58], &edges[63]);
    edges[58].set_adjacent_edge(&edges[51], &edges[57], &edges[59], &edges[64]);
    edges[59].set_adjacent_edge(&edges[52], &edges[58], &edges[60], &edges[64]);
    edges[60].set_adjacent_edge(&edges[52], &edges[59], &edges[61], &edges[65]);
    edges[61].set_adjacent_edge(&edges[53], &edges[60], &edges[65], nullptr);

    edges[54].set_adjacent_vertex(&vertices[38], &vertices[43]);
    edges[55].set_adjacent_vertex(&vertices[39], &vertices[43]);
    edges[56].set_adjacent_vertex(&vertices[39], &vertices[44]);
    edges[57].set_adjacent_vertex(&vertices[40], &vertices[44]);
    edges[58].set_adjacent_vertex(&vertices[40], &vertices[45]);
    edges[59].set_adjacent_vertex(&vertices[41], &vertices[45]);
    edges[60].set_adjacent_vertex(&vertices[41], &vertices[46]);
    edges[61].set_adjacent_vertex(&vertices[42], &vertices[46]);

    // tenth edge row
    edges[62].set_adjacent_edge(&edges[54], &edges[55], &edges[66], nullptr);
    edges[63].set_adjacent_edge(&edges[56], &edges[57], &edges[67], &edges[68]);
    edges[64].set_adjacent_edge(&edges[58], &edges[59], &edges[69], &edges[70]);
    edges[65].set_adjacent_edge(&edges[60], &edges[61], &edges[71], nullptr);

    edges[62].set_adjacent_vertex(&vertices[43], &vertices[47]);
    edges[63].set_adjacent_vertex(&vertices[44], &vertices[48]);
    edges[64].set_adjacent_vertex(&vertices[45], &vertices[49]);
    edges[65].set_adjacent_vertex(&vertices[46], &vertices[50]);

    // eleventh edge row
    edges[66].set_adjacent_edge(&edges[62], &edges[67], nullptr, nullptr);
    edges[67].set_adjacent_edge(&edges[63], &edges[66], &edges[68], nullptr);
    edges[68].set_adjacent_edge(&edges[63], &edges[67], &edges[69], nullptr);
    edges[69].set_adjacent_edge(&edges[64], &edges[68], &edges[70], nullptr);
    edges[70].set_adjacent_edge(&edges[64], &edges[69], &edges[71], nullptr);
    edges[71].set_adjacent_edge(&edges[65], &edges[70], nullptr, nullptr);

    edges[66].set_adjacent_vertex(&vertices[47], &vertices[51]);
    edges[67].set_adjacent_vertex(&vertices[48], &vertices[51]);
    edges[68].set_adjacent_vertex(&vertices[48], &vertices[52]);
    edges[69].set_adjacent_vertex(&vertices[49], &vertices[52]);
    edges[70].set_adjacent_vertex(&vertices[49], &vertices[53]);
    edges[71].set_adjacent_vertex(&vertices[50], &vertices[53]);
}

void Catan::init_board() {
    // first row
    vertices[0].set_resource(resource::STONE, 10, resource::NONE, 0, resource::NONE, 0);
    vertices[1].set_resource(resource::SHEEP, 2, resource::NONE, 0, resource::NONE, 0);
    vertices[2].set_resource(resource::WOOD, 9, resource::NONE, 0, resource::NONE, 0);

    // second row
    vertices[3].set_resource(resource::STONE, 10, resource::NONE, 0, resource::NONE, 0);
    vertices[4].set_resource(resource::STONE, 10, resource::SHEEP, 2, resource::NONE, 0);
    vertices[5].set_resource(resource::SHEEP, 2, resource::WOOD, 9, resource::NONE, 0);
    vertices[6].set_resource(resource::WOOD, 9, resource::NONE, 0, resource::NONE, 0);

    // third row
    vertices[7].set_resource(resource::WHEAT, 12, resource::STONE, 10, resource::NONE, 0);
    vertices[8].set_resource(resource::CLAY, 6, resource::STONE, 10, resource::SHEEP, 2);
    vertices[9].set_resource(resource::SHEEP, 4, resource::SHEEP, 2, resource::WOOD, 9);
    vertices[10].set_resource(resource::CLAY, 10, resource::WOOD, 9, resource::NONE, 0);

    // fourth row
    vertices[11].set_resource(resource::WHEAT, 12, resource::NONE, 0, resource::NONE, 0);
    vertices[12].set_resource(resource::WHEAT, 12, resource::CLAY, 6, resource::STONE, 10);
    vertices[13].set_resource(resource::CLAY, 6, resource::SHEEP, 2, resource::SHEEP, 4);
    vertices[14].set_resource(resource::WOOD, 9, resource::CLAY, 10, resource::SHEEP, 4);
    vertices[15].set_resource(resource::CLAY, 10, resource::NONE, 0, resource::NONE, 0);

    // fifth row
    vertices[16].set_resource(resource::WHEAT, 9, resource::WHEAT, 12, resource::NONE, 0);
    vertices[17].set_resource(resource::WOOD, 11, resource::WHEAT, 12, resource::CLAY, 6);
    vertices[18].set_resource(resource::DESERT, 0, resource::CLAY, 6, resource::SHEEP, 4);
    vertices[19].set_resource(resource::WOOD, 3, resource::SHEEP, 4, resource::CLAY, 10);
    vertices[20].set_resource(resource::STONE, 8, resource::CLAY, 10, resource::NONE, 0);

    // sixth row
    vertices[21].set_resource(resource::WHEAT, 9, resource::NONE, 0, resource::NONE, 0);
    vertices[22].set_resource(resource::WHEAT, 9, resource::WOOD, 11, resource::WHEAT, 12);
    vertices[23].set_resource(resource::WOOD, 11, resource::CLAY, 6, resource::DESERT, 0);
    vertices[24].set_resource(resource::SHEEP, 4, resource::DESERT, 0, resource::WOOD, 3);
    vertices[25].set_resource(resource::STONE, 8, resource::WOOD, 3, resource::CLAY, 10);
    vertices[26].set_resource(resource::STONE, 8, resource::NONE, 0, resource::NONE, 0);

    // seventh row
    vertices[27].set_resource(resource::WHEAT, 9, resource::NONE, 0, resource::NONE, 0);
    vertices[28].set_resource(resource::WOOD, 8, resource::WHEAT, 9, resource::WOOD, 11);
    vertices[29].set_resource(resource::STONE, 3, resource::WOOD, 11, resource::DESERT, 0);
    vertices[30].set_resource(resource::WHEAT, 4, resource::DESERT, 0, resource::WOOD, 3);
    vertices[31].set_resource(resource::SHEEP, 5, resource::WOOD, 3, resource::STONE, 8);
    vertices[32].set_resource(resource::STONE, 8, resource::NONE, 0, resource::NONE, 0);

    // eighth row
    vertices[33].set_resource(resource::WHEAT, 9, resource::WOOD, 8, resource::NONE, 0);
    vertices[34].set_resource(resource::WOOD, 8, resource::STONE, 3, resource::WOOD, 11);
    vertices[35].set_resource(resource::STONE, 3, resource::WHEAT, 4, resource::DESERT, 0);
    vertices[36].set_resource(resource::WHEAT, 4, resource::SHEEP, 5, resource::WOOD, 3);
    vertices[37].set_resource(resource::SHEEP, 5, resource::STONE, 8, resource::NONE, 0);

    // ninth row
    vertices[38].set_resource(resource::WOOD, 8, resource::NONE, 0, resource::NONE, 0);
    vertices[39].set_resource(resource::CLAY, 5, resource::WOOD, 8, resource::STONE, 3);
    vertices[40].set_resource(resource::WHEAT, 6, resource::STONE, 3, resource::WHEAT, 4);
    vertices[41].set_resource(resource::SHEEP, 11, resource::WHEAT, 4, resource::SHEEP, 5);
    vertices[42].set_resource(resource::SHEEP, 5, resource::NONE, 0, resource::NONE, 0);

    // tenth row
    vertices[43].set_resource(resource::CLAY, 5, resource::WOOD, 8, resource::NONE, 0);
    vertices[44].set_resource(resource::WHEAT, 6, resource::CLAY, 5, resource::STONE, 3);
    vertices[45].set_resource(resource::SHEEP, 11, resource::WHEAT, 6, resource::WHEAT, 4);
    vertices[46].set_resource(resource::SHEEP, 5, resource::SHEEP, 11, resource::NONE, 0);

    // eleventh row
    vertices[47].set_resource(resource::CLAY, 5, resource::NONE, 0, resource::NONE, 0);
    vertices[48].set_resource(resource::CLAY, 5, resource::WHEAT, 6, resource::NONE, 0);
    vertices[49].set_resource(resource::WHEAT, 6, resource::SHEEP, 11, resource::NONE, 0);
    vertices[50].set_resource(resource::SHEEP, 11, resource::NONE, 0, resource::NONE, 0);

    // twelfth row
    vertices[51].set_resource(resource::CLAY, 5, resource::NONE, 0, resource::NONE, 0);
    vertices[52].set_resource(resource::WHEAT, 6, resource::NONE, 0, resource::NONE, 0);
    vertices[53].set_resource(resource::SHEEP, 11, resource::NONE, 0, resource::NONE, 0);
}

void Catan::init_dev_cards() {
    for (int i = 0; i < 3; i++) {
        dev_cards.push_back(new KnightCard());
    }
    for (int i = 0; i < 2; i++) {
        dev_cards.push_back(new RoadBuildCard());
        dev_cards.push_back(new YearOfPlentyCard());
        dev_cards.push_back(new MonopolyCard());
    }

    for (int i = 0; i < 5; i++) {
        dev_cards.push_back(new VictoryPointCard());
    }
}

void Catan::display_board() {
    cout << "            " << vertices[0].get_settlement_string() << "      " << vertices[1].get_settlement_string() << "       " << vertices[2].get_settlement_string() << "\n"
         << "          " << edges[0].get_color_code() << "/   " << edges[1].get_color_code() << "\\  " << edges[2].get_color_code() << "/   " << edges[3].get_color_code() << "\\   " << edges[4].get_color_code() << "/   " << edges[5].get_color_code() << "\\\033[0m\n"
         << "         " << vertices[3].get_settlement_string() << "     " << vertices[4].get_settlement_string() << "       " << vertices[5].get_settlement_string() << "      " << vertices[6].get_settlement_string() << "\n"
         << "         " << edges[6].get_color_code() << "| " << vertices[0].get_resources()[0].first.get_emoji() << edges[7].get_color_code() << "  |" << "  " << vertices[1].get_resources()[0].first.get_emoji() << edges[8].get_color_code() << "   |  " << vertices[2].get_resources()[0].first.get_emoji() << "  " << edges[9].get_color_code() << "|\033[0m\n"
         << "         " << vertices[7].get_settlement_string() << " " << vertices[0].get_resources()[0].second << "  " << vertices[8].get_settlement_string() << "   " << vertices[1].get_resources()[0].second << "   " << vertices[9].get_settlement_string() << "   " << vertices[2].get_resources()[0].second << "  " << vertices[10].get_settlement_string() << "\n"
         << "       " << edges[10].get_color_code() << "/   " << edges[11].get_color_code() << "\\ " << edges[12].get_color_code() << "/   " << edges[13].get_color_code() << "\\   " << edges[14].get_color_code() << "/   " << edges[15].get_color_code() << "\\  " << edges[16].get_color_code() << "/   " << edges[17].get_color_code() << "\\\033[0m\n"
         << "      " << vertices[11].get_settlement_string() << "     " << vertices[12].get_settlement_string() << "      " << vertices[13].get_settlement_string() << "      " << vertices[14].get_settlement_string() << "      " << vertices[15].get_settlement_string() << "\n"
         << "      " << edges[18].get_color_code() << "| " << vertices[7].get_resources()[0].first.get_emoji() << edges[19].get_color_code() << "  |  " << vertices[8].get_resources()[0].first.get_emoji() << edges[20].get_color_code() << "  |  " << vertices[9].get_resources()[0].first.get_emoji() << edges[21].get_color_code() << "  |  " << vertices[10].get_resources()[0].first.get_emoji() << "  " << edges[22].get_color_code() << "|\033[0m\n"
         << "      " << vertices[16].get_settlement_string() << " " << vertices[7].get_resources()[0].second << "  " << vertices[17].get_settlement_string() << "   " << vertices[8].get_resources()[0].second << "   " << vertices[18].get_settlement_string() << "  " << vertices[9].get_resources()[0].second << "  " << vertices[19].get_settlement_string() << "  " << vertices[10].get_resources()[0].second << "  " << vertices[20].get_settlement_string() << "\n"
         << "    " << edges[23].get_color_code() << "/   " << edges[24].get_color_code() << "\\ " << edges[25].get_color_code() << "/   " << edges[26].get_color_code() << "\\   " << edges[27].get_color_code() << "/   " << edges[28].get_color_code() << "\\ " << edges[29].get_color_code() << "/   " << edges[30].get_color_code() << "\\  " << edges[31].get_color_code() << "/   " << edges[32].get_color_code() << "\\\033[0m\n"
         << "   " << vertices[21].get_settlement_string() << "     " << vertices[22].get_settlement_string() << "      " << vertices[23].get_settlement_string() << "      " << vertices[24].get_settlement_string() << "     " << vertices[25].get_settlement_string() << "      " << vertices[26].get_settlement_string() << "\n"
         << "   " << edges[33].get_color_code() << "| " << vertices[16].get_resources()[0].first.get_emoji() << edges[34].get_color_code() << "  |  " << vertices[17].get_resources()[0].first.get_emoji() << edges[35].get_color_code() << "  |  " << vertices[18].get_resources()[0].first.get_emoji() << edges[36].get_color_code() << "   | " << vertices[19].get_resources()[0].first.get_emoji() << edges[37].get_color_code() << "  |  " << vertices[20].get_resources()[0].first.get_emoji() << "  " << edges[38].get_color_code() << "|\033[0m\n"
         << "   " << vertices[27].get_settlement_string() << "  " << vertices[16].get_resources()[0].second << "  " << vertices[28].get_settlement_string() << "  " << vertices[17].get_resources()[0].second << "  " << vertices[29].get_settlement_string() << "   " << vertices[18].get_resources()[0].second << "  " << vertices[30].get_settlement_string() << "  " << vertices[19].get_resources()[0].second << "  " << vertices[31].get_settlement_string() << "   " << vertices[20].get_resources()[0].second << "  " << vertices[32].get_settlement_string() << "\n"
         << "    " << edges[38].get_color_code() << "\\   " << edges[40].get_color_code() << "/ " << edges[41].get_color_code() << "\\    " << edges[42].get_color_code() << "/   " << edges[43].get_color_code() << "\\ " << edges[44].get_color_code() << "/   " << edges[45].get_color_code() << "\\ " << edges[46].get_color_code() << "/   " << edges[47].get_color_code() << "\\   " << edges[48].get_color_code() << "/\033[0m\n"
         << "      " << vertices[33].get_settlement_string() << "     " << vertices[34].get_settlement_string() << "      " << vertices[35].get_settlement_string() << "      " << vertices[36].get_settlement_string() << "      " << vertices[37].get_settlement_string() << "\n"
         << "      " << edges[49].get_color_code() << "| " << vertices[28].get_resources()[0].first.get_emoji() << edges[50].get_color_code() << "  |  " << vertices[29].get_resources()[0].first.get_emoji() << edges[51].get_color_code() << "  |  " << vertices[30].get_resources()[0].first.get_emoji() << edges[52].get_color_code() << "  |  " << vertices[31].get_resources()[0].first.get_emoji() << edges[53].get_color_code() << "  |\033[0m\n"
         << "      " << vertices[38].get_settlement_string() << "  " << vertices[28].get_resources()[0].second << "  " << vertices[39].get_settlement_string() << "   " << vertices[29].get_resources()[0].second << "  " << vertices[40].get_settlement_string() << "   " << vertices[30].get_resources()[0].second << "  " << vertices[41].get_settlement_string() << "   " << vertices[31].get_resources()[0].second << "  " << vertices[42].get_settlement_string() << "\n"
         << "       " << edges[54].get_color_code() << "\\   " << edges[55].get_color_code() << "/  " << edges[56].get_color_code() << "\\   " << edges[57].get_color_code() << "/  " << edges[58].get_color_code() << "\\   " << edges[59].get_color_code() << "/  " << edges[60].get_color_code() << "\\   " << edges[61].get_color_code() << "/\033[0m\n"
         << "         " << vertices[43].get_settlement_string() << "      " << vertices[44].get_settlement_string() << "      " << vertices[45].get_settlement_string() << "      " << vertices[46].get_settlement_string() << "\n"
         << "         " << edges[62].get_color_code() << "|  " << vertices[39].get_resources()[0].first.get_emoji() << edges[63].get_color_code() << "  |  " << vertices[40].get_resources()[0].first.get_emoji() << edges[64].get_color_code() << "  |  " << vertices[41].get_resources()[0].first.get_emoji() << edges[65].get_color_code() << "  |\033[0m\n"
         << "         " << vertices[47].get_settlement_string() << "   " << vertices[39].get_resources()[0].second << "  " << vertices[48].get_settlement_string() << "   " << vertices[40].get_resources()[0].second << "  " << vertices[49].get_settlement_string() << "   " << vertices[41].get_resources()[0].second << " " << vertices[50].get_settlement_string() << "\n"
         << "           " << edges[66].get_color_code() << "\\   " << edges[67].get_color_code() << "/  " << edges[68].get_color_code() << "\\   " << edges[69].get_color_code() << "/  " << edges[70].get_color_code() << "\\   " << edges[71].get_color_code() << "/\033[0m\n"
         << "             " << vertices[51].get_settlement_string() << "      " << vertices[52].get_settlement_string() << "       " << vertices[53].get_settlement_string() << "\n";
}

// void Catan::display_board_ids() {
//     cout << "             " << vertices[0].get_id() << "        " << vertices[1].get_id() << "         " << vertices[2].get_id() << "\n"
//          << "          " << edges[0].get_id() << "/   " << edges[1].get_id() << "\\  " << edges[2].get_id() << "/   " << edges[3].get_id() << "\\   " << edges[4].get_id() << "/   " << edges[5].get_id() << "\\\n"
//          << "          " << vertices[3].get_id() << "       " << vertices[4].get_id() << "        " << vertices[5].get_id() << "        " << vertices[6].get_id() << "\n"
//          << "         " << edges[6].get_id() << "|      " << edges[7].get_id() << "|       " << edges[8].get_id() << "|       " << edges[9].get_id() << "|   \n"
//          << "          " << vertices[7].get_id() << "       " << vertices[8].get_id() << "        " << vertices[9].get_id() << "        " << vertices[10].get_id() << "\n"
//          << "      " << edges[10].get_id() << "/ " << edges[11].get_id() << "\\  " << edges[12].get_id() << "/ " << edges[13].get_id() << "\\   " << edges[14].get_id() << "/" << edges[15].get_id() << "\\  " << edges[16].get_id() << "/" << edges[17].get_id() << "\\\n"
//          << "     " << vertices[11].get_id() << "       " << vertices[12].get_id() << "      " << vertices[13].get_id() << "     " << vertices[14].get_id() << "      " << vertices[15].get_id() << "\n"
//          << "     " << edges[18].get_id() << "|    " << edges[19].get_id() << "|      " << edges[20].get_id() << "|    " << edges[21].get_id() << "|   \n"
//          << "     " << vertices[16].get_id() << "       " << vertices[17].get_id() << "        " << vertices[18].get_id() << "        " << vertices[19].get_id() << "      " << vertices[20].get_id() << "\n"
//          << "   " << edges[23].get_id() << "/ " << edges[24].get_id() << "\\  " << edges[25].get_id() << "/ " << edges[26].get_id() << "\\   " << edges[27].get_id() << "/" << edges[28].get_id() << "\\  " << edges[29].get_id() << "/" << edges[30].get_id() << "\\  " << edges[31].get_id() << "/" << edges[32].get_id() << "\\\n"
//          << "  " << vertices[21].get_id() << "       " << vertices[22].get_id() << "        " << vertices[23].get_id() << "        " << vertices[24].get_id() << "        " << vertices[25].get_id() << "        " << vertices[26].get_id() << "\n"
//          << "  " << edges[33].get_id() << "|      " << edges[34].get_id() << "|       " << edges[35].get_id() << "|       " << edges[36].get_id() << "|       " << edges[37].get_id() << "|   \n"
//          << "  " << vertices[27].get_id() << "       " << vertices[28].get_id() << "        " << vertices[29].get_id() << "        " << vertices[30].get_id() << "        " << vertices[31].get_id() << "        " << vertices[32].get_id() << "\n"
//          << "   " << edges[38].get_id() << "\\  " << edges[40].get_id() << "/ " << edges[41].get_id() << "\\   " << edges[42].get_id() << "/" << edges[43].get_id() << "\\  " << edges[44].get_id() << "/" << edges[45].get_id() << "\\  " << edges[46].get_id() << "/" << edges[47].get_id() << "\\  " << edges[48].get_id() << "/\n"
//          << "     " << vertices[33].get_id() << "        " << vertices[34].get_id() << "        " << vertices[35].get_id() << "        " << vertices[36].get_id() << "        " << vertices[37].get_id() << "\n"
//          << "     " << edges[49].get_id() << "|      " << edges[50].get_id() << "|       " << edges[51].get_id() << "|       " << edges[52].get_id() << "|       " << edges[53].get_id() << "|   \n"
//          << "     " << vertices[38].get_id() << "       " << vertices[39].get_id() << "        " << vertices[40].get_id() << "        " << vertices[41].get_id() << "        " << vertices[42].get_id() << "\n"
//          << "      " << edges[54].get_id() << "\\  " << edges[55].get_id() << "/  " << edges[56].get_id() << "\\   " << edges[57].get_id() << "/" << edges[58].get_id() << "\\  " << edges[59].get_id() << "/" << edges[60].get_id() << "\\  " << edges[61].get_id() << "/\n"
//          << "        " << vertices[43].get_id() << "        " << vertices[44].get_id() << "        " << vertices[45].get_id() << "        " << vertices[46].get_id() << "\n"
//          << "        " << edges[62].get_id() << "|       " << edges[63].get_id() << "|       " << edges[64].get_id() << "|       " << edges[65].get_id() << "|   \n"
//          << "        " << vertices[47].get_id() << "        " << vertices[48].get_id() << "        " << vertices[49].get_id() << "        " << vertices[50].get_id() << "\n"
//          << "          " << edges[66].get_id() << "\\  " << edges[67].get_id() << "/  " << edges[68].get_id() << "\\   " << edges[69].get_id() << "/" << edges[70].get_id() << "\\  " << edges[71].get_id() << "/\n"
//          << "            " << vertices[51].get_id() << "        " << vertices[52].get_id() << "         " << vertices[53].get_id() << "\n";

//     ;
// }