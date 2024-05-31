#include <iostream>

#include "Catan.hpp"
#include "player/Player.hpp"

using namespace std;

int main() {
    Player player1(PlayerColor::RED);
    Player player2(PlayerColor::BLUE);
    Player player3(PlayerColor::YELLOW);

    Catan catan(player1, player2, player3);
    cout << "~~~ Game initialized ~~~" << endl;

    catan.print_cell_status(0);
    catan.print_cell_status(8);

    return 0;
}