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

int main() {
    // Player player1(PlayerColor::RED);
    // Player player2(PlayerColor::BLUE);
    // Player player3(PlayerColor::YELLOW);
    // player2.add_resource(resource::STONE, 3);
    // player2.add_resource(resource::WHEAT, 3);
    // Catan catan(player1, player2, player3);
    // catan.display_board();
    // catan.first_round();

    // while (catan.is_game_over() == nullptr) {
    //     catan.play_turn();
    // }
    place_things();
    return 0;
}
