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

void place_a_city(Catan& catan, Player& player){
    player.add_resource(resource::WHEAT,3);
    player.add_resource(resource::STONE,3);

    player.display_resources();
    catan.place_city(31, player);
    player.display_resources();
}

int main() {
    Player player1(PlayerColor::RED);
    Player player2(PlayerColor::BLUE);
    Player player3(PlayerColor::YELLOW);

    Catan catan(player1, player2, player3);
    // catan.first_round();
    demo_first_round(catan, player1, player2, player3);
    // place_a_city(catan, player2);
    catan.display_board();
    // catan.roll_dice();

    // Player* winner;
    // while ((winner = catan.is_game_over()) == nullptr) {
    //     catan.play_turn();
    // }

    // winner = catan.start_game();
    // cout << "Game over! " << winner->get_color() << " wins!" << endl;

    // catan.give_resources(8);
    // player1.display_resources();

    return 0;
}
