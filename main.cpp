#include <iostream>

#include "Catan.hpp"
#include "player/Player.hpp"

using namespace std;

int main() {
    Player player1(PlayerColor::RED);
    Player player2(PlayerColor::BLUE);
    Player player3(PlayerColor::YELLOW);
    player2.add_resource(resource::STONE, 3);
    player2.add_resource(resource::WHEAT, 3);
    Catan catan(player1, player2, player3);
    // cout << (catan.vertices[0].get_adjacent_edge(0) == nullptr) << endl;
    cout << "~~~ Game initialized ~~~" << endl;
    // catan.place_settlement(0, player1, 1);
    // catan.place_settlement(29, player2, 1);
    catan.place_settlement(49, player2, 1);
    player2.display_resources();
    player2.place_city(catan);
    player2.display_resources();
    // catan.place_settlement(52, player3, 1);
    catan.display_board();
    return 0;
}
