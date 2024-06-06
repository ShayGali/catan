#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <iostream>

#include "../Catan.hpp"
#include "../player/Player.hpp"
#include "doctest.h"
using namespace std;

TEST_CASE("place settlement") {
    SUBCASE("place in first round") {
        Player player1(PlayerColor::RED);
        Player player2(PlayerColor::BLUE);
        Player player3(PlayerColor::YELLOW);
        Catan catan(player1, player2, player3);

        catan.place_settlement(8, player1, true);
        CHECK(catan.get_vertices()[8].get_owner() == &player1);
        CHECK(player1.get_victory_points() == 1);
    }
}