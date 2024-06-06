#include <iostream>

#include "Catan.hpp"
#include "player/Player.hpp"

using namespace std;

void place_things() {
    Player player1(PlayerColor::RED);
    Catan catan(player1, player1, player1);
    int index = 0;
    while (index != -1) {
        try {
            catan.display_board();
            cout << "settlement index: ";
            cin >> index;
            catan.place_settlement(index, player1, true);
            catan.display_board();
            cout << "road index: ";
            cin >> index;
            catan.place_road(index, player1, true);

            catan.display_board();
        } catch (const std::exception& e) {
            cout << e.what() << endl;
        }
    }
}

void demo_first_round(Catan& catan, Player& player1, Player& player2, Player& player3) {
    // 8 13 20 31 31 46 47 62 14 15 21 23
    catan.place_settlement(8, player1, true);
    catan.place_road(13, player1, true);

    catan.place_settlement(20, player1, true);
    catan.place_road(31, player1, true);

    player1.add_resource(resource::CLAY, 1);
    player1.add_resource(resource::STONE, 1);

    catan.place_settlement(31, player2, true);
    catan.place_road(46, player2, true);

    catan.place_settlement(47, player2, true);
    catan.place_road(62, player2, true);

    player2.add_resource(resource::CLAY, 1);

    catan.place_settlement(14, player3, true);
    catan.place_road(15, player3, true);

    catan.place_settlement(21, player3, true);
    catan.place_road(23, player3, true);

    player3.add_resource(resource::WHEAT, 1);
}

void place_a_city(Catan& catan, Player& player) {
    player.add_resource(resource::WHEAT, 3);
    player.add_resource(resource::STONE, 3);

    player.display_resources();
    catan.place_city(31, player);
    player.display_resources();
}

void real_game() {
    Player player1(PlayerColor::RED);
    Player player2(PlayerColor::BLUE);
    Player player3(PlayerColor::YELLOW);

    Catan catan(player1, player2, player3);
    Player* winner = catan.start_game();
    cout << "Game over! " << winner->get_color() << " wins!" << endl;
}

void real_game_with_demo_start() {
    Player player1(PlayerColor::RED);
    Player player2(PlayerColor::BLUE);
    Player player3(PlayerColor::YELLOW);

    Catan catan(player1, player2, player3);
    // 8 13 20 31 31 46 47 62 14 15 21 23
    catan.place_settlement(8, player1, true);
    catan.place_road(13, player1, true);

    catan.place_settlement(20, player1, true);
    catan.place_road(31, player1, true);

    player1.add_resource(resource::CLAY, 1);
    player1.add_resource(resource::STONE, 1);

    catan.place_settlement(31, player2, true);
    catan.place_road(46, player2, true);

    catan.place_settlement(47, player2, true);
    catan.place_road(62, player2, true);

    player2.add_resource(resource::CLAY, 1);

    catan.place_settlement(14, player3, true);
    catan.place_road(15, player3, true);

    catan.place_settlement(21, player3, true);
    catan.place_road(23, player3, true);

    player3.add_resource(resource::WHEAT, 1);

    player1.add_resource(resource::WHEAT, 2);
    player1.add_resource(resource::SHEEP, 1);
    player1.add_resource(resource::STONE, 3);

    Player* winner;
    while ((winner = catan.is_game_over()) == nullptr) {
        catan.play_turn();
    }
    cout << "Game over! " << winner->get_color() << " wins!" << endl;
}

int main() {
    real_game_with_demo_start();
    // real_game();
    return 0;
}
