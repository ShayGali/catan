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

    SUBCASE("place near a road") {
        Player player1(PlayerColor::RED);
        Catan catan(player1, player1, player1);

        // give player1 a resource to make him able to place a settlement
        player1.add_resource(resource::WOOD, 3);
        player1.add_resource(resource::CLAY, 3);
        player1.add_resource(resource::SHEEP, 2);
        player1.add_resource(resource::WHEAT, 2);

        catan.place_settlement(34, player1, true);
        catan.place_road(41, player1);
        catan.place_road(40, player1);
        catan.place_settlement(33, player1);

        CHECK(catan.get_vertices()[33].get_owner() == &player1);
        CHECK(player1.get_victory_points() == 2);
    }

    SUBCASE("place near a road that is not owned by the player") {
        Player player1(PlayerColor::RED);
        Player player2(PlayerColor::BLUE);
        Catan catan(player1, player2, player2);

        // give player2 a resource to make him able to place a settlement
        player2.add_resource(resource::WOOD, 1);
        player2.add_resource(resource::CLAY, 1);
        player2.add_resource(resource::SHEEP, 1);
        player2.add_resource(resource::WHEAT, 1);

        catan.place_settlement(34, player1, true);
        catan.place_road(41, player1, true);
        catan.place_road(40, player1, true);
        CHECK_THROWS(catan.place_settlement(33, player2));
    }

    SUBCASE("try to place in taken vertex") {
        Player player1(PlayerColor::RED);
        Player player2(PlayerColor::BLUE);
        Player player3(PlayerColor::YELLOW);
        Catan catan(player1, player2, player3);

        // give player2 a resource to make him able to place a settlement
        player2.add_resource(resource::WOOD, 1);
        player2.add_resource(resource::CLAY, 1);
        player2.add_resource(resource::SHEEP, 1);
        player2.add_resource(resource::WHEAT, 1);

        catan.place_settlement(8, player1, true);
        CHECK_THROWS(catan.place_settlement(8, player2));
        // check that the vertex is still owned by player1
        CHECK(catan.get_vertices()[8].get_owner() == &player1);
        // check that player2 still have the resources
        CHECK(player2.get_resource_count(resource::WOOD) == 1);
        CHECK(player2.get_resource_count(resource::CLAY) == 1);
        CHECK(player2.get_resource_count(resource::SHEEP) == 1);
        CHECK(player2.get_resource_count(resource::WHEAT) == 1);
    }

    SUBCASE("try to place near settlement") {
        Player player1(PlayerColor::RED);
        Player player2(PlayerColor::BLUE);
        Player player3(PlayerColor::YELLOW);
        Catan catan(player1, player2, player3);

        catan.place_settlement(8, player1, true);
        CHECK_THROWS(catan.place_settlement(4, player2, true));
        // check that the vertex is still owned by player1
        CHECK(catan.get_vertices()[8].get_owner() == &player1);
    }

    SUBCASE("place without any resources") {
        Player player1(PlayerColor::RED);
        Player player2(PlayerColor::BLUE);
        Player player3(PlayerColor::YELLOW);
        Catan catan(player1, player2, player3);

        CHECK_THROWS(catan.place_settlement(8, player1));
    }
}

TEST_CASE("place road") {
    SUBCASE("place in first round") {
        Player player1(PlayerColor::RED);
        Player player2(PlayerColor::BLUE);
        Player player3(PlayerColor::YELLOW);
        Catan catan(player1, player2, player3);

        catan.place_settlement(8, player1, true);
        catan.place_road(7, player1, true);
        CHECK(catan.get_edges()[7].get_owner() == &player1);
    }

    SUBCASE("check if the resources are taken from the player") {
        Player player1(PlayerColor::RED);
        Player player2(PlayerColor::BLUE);
        Player player3(PlayerColor::YELLOW);
        Catan catan(player1, player2, player3);

        player1.add_resource(resource::WOOD, 1);
        player1.add_resource(resource::CLAY, 1);

        catan.place_settlement(8, player1, true);
        catan.place_road(7, player1);
        CHECK((player1.get_resource_count(resource::WOOD) == 0 && player1.get_resource_count(resource::CLAY) == 0));
    }

    SUBCASE("try to place in taken edge") {
        Player player1(PlayerColor::RED);
        Player player2(PlayerColor::BLUE);
        Player player3(PlayerColor::YELLOW);
        Catan catan(player1, player2, player3);

        catan.place_settlement(8, player1, true);
        catan.place_road(7, player1, true);
        CHECK_THROWS(catan.place_road(7, player2, true));

        // check that the edge is still owned by player1
        CHECK(catan.get_edges()[7].get_owner() == &player1);
    }

    SUBCASE("place a road near a another road") {
        Player player1(PlayerColor::RED);
        Player player2(PlayerColor::BLUE);
        Player player3(PlayerColor::YELLOW);
        Catan catan(player1, player2, player3);

        catan.place_settlement(8, player1, true);
        catan.place_road(7, player1, true);
        catan.place_road(2, player1, true);
        catan.place_road(3, player1, true);
        catan.place_road(4, player1, true);
        CHECK((catan.get_edges()[2].get_owner() == &player1 && catan.get_edges()[3].get_owner() == &player1 && catan.get_edges()[4].get_owner() == &player1));
    }

    SUBCASE("try to place a road without any resources") {
        Player player1(PlayerColor::RED);
        Player player2(PlayerColor::BLUE);
        Player player3(PlayerColor::YELLOW);
        Catan catan(player1, player2, player3);

        CHECK_THROWS(catan.place_road(7, player1));
    }

    SUBCASE("try to place a road without near road or settlement") {
        Player player1(PlayerColor::RED);
        Player player2(PlayerColor::BLUE);
        Player player3(PlayerColor::YELLOW);
        Catan catan(player1, player2, player3);

        CHECK_THROWS(catan.place_road(7, player1));
    }
}

TEST_CASE("place city") {
    SUBCASE("simple place city") {
        Player player1(PlayerColor::RED);
        Player player2(PlayerColor::BLUE);
        Player player3(PlayerColor::YELLOW);
        Catan catan(player1, player2, player3);

        // give player1 a resource to make him able to place a settlement
        player1.add_resource(resource::WHEAT, 2);
        player1.add_resource(resource::STONE, 3);

        catan.place_settlement(8, player1, true);
        catan.place_city(8, player1);

        CHECK(catan.get_vertices()[8].get_owner() == &player1);
        CHECK(catan.get_vertices()[8].get_isCity());
        CHECK(player1.get_victory_points() == 2);

        // check that the resources are taken from the player
        CHECK((player1.get_resource_count(resource::WHEAT) == 0 && player1.get_resource_count(resource::STONE) == 0));
    }

    SUBCASE("try to place city without any resources") {
        Player player1(PlayerColor::RED);
        Player player2(PlayerColor::BLUE);
        Player player3(PlayerColor::YELLOW);
        Catan catan(player1, player2, player3);
        catan.place_settlement(8, player1, true);
        CHECK_THROWS(catan.place_city(8, player1));
    }

    SUBCASE("try to place city in a settlement that is not owned by the player") {
        Player player1(PlayerColor::RED);
        Player player2(PlayerColor::BLUE);
        Player player3(PlayerColor::YELLOW);
        Catan catan(player1, player2, player3);
        catan.place_settlement(8, player1, true);

        // give player2 a resource to make him able to place a settlement
        player2.add_resource(resource::WHEAT, 2);
        player2.add_resource(resource::STONE, 3);

        // the vertex is owned by player1
        CHECK_THROWS(catan.place_city(8, player1));
        // the vertex owner is null
        CHECK_THROWS(catan.place_city(6, player1));

        // check that the vertex is still owned by player1 and it is not a city
        CHECK((catan.get_vertices()[8].get_owner() == &player1 && !catan.get_vertices()[8].get_isCity()));
    }
}

TEST_CASE("give resources on roll") {
    Player player1(PlayerColor::RED);
    Player player2(PlayerColor::BLUE);
    Player player3(PlayerColor::YELLOW);
    Catan catan(player1, player2, player3);

    // give player1 a resource to make him able to place a city
    player1.add_resource(resource::WHEAT, 2);
    player1.add_resource(resource::STONE, 3);

    catan.place_settlement(44, player1, true);  // STONE:3, WHEAT:6, CLAY:5
    catan.place_city(44, player1);
    catan.place_settlement(45, player1, true);  // WHEAT:6, WHEAT:4, SHEEP:11

    catan.place_settlement(8, player2, true);  // CLAY:6, STONE:10, SHEEP:2

    // roll 10
    catan.give_resources(10);

    // check that the resources are given to the players
    CHECK((player1.get_resource_count(resource::WHEAT) == 0 && player1.get_resource_count(resource::STONE) == 0 && player1.get_resource_count(resource::CLAY) == 0 && player1.get_resource_count(resource::SHEEP) == 0 && player1.get_resource_count(resource::WOOD) == 0));
    CHECK((player2.get_resource_count(resource::WHEAT) == 0 && player2.get_resource_count(resource::STONE) == 1 && player2.get_resource_count(resource::CLAY) == 0 && player2.get_resource_count(resource::SHEEP) == 0 && player1.get_resource_count(resource::WOOD) == 0));

    // roll 6
    catan.give_resources(6);

    // check that the resources are given to the players
    CHECK((player1.get_resource_count(resource::WHEAT) == 3 && player1.get_resource_count(resource::STONE) == 0 && player1.get_resource_count(resource::CLAY) == 0 && player1.get_resource_count(resource::SHEEP) == 0 && player1.get_resource_count(resource::WOOD) == 0));
    CHECK((player2.get_resource_count(resource::WHEAT) == 0 && player2.get_resource_count(resource::STONE) == 1 && player2.get_resource_count(resource::CLAY) == 1 && player2.get_resource_count(resource::SHEEP) == 0 && player1.get_resource_count(resource::WOOD) == 0));
}

TEST_CASE("Buy dev card") {
    SUBCASE("simple buy dev card") {
        Player player1(PlayerColor::RED);
        Catan catan(player1, player1, player1);

        // give player1 a resource to make him able to buy a dev card
        player1.add_resource(resource::WHEAT, 1);
        player1.add_resource(resource::SHEEP, 1);
        player1.add_resource(resource::STONE, 1);

        player1.buy_dev_card(catan);

        // check that the resources are taken from the player
        CHECK((player1.get_resource_count(resource::WHEAT) == 0 && player1.get_resource_count(resource::SHEEP) == 0 && player1.get_resource_count(resource::STONE) == 0));
        // check that the player has a dev card
        CHECK(player1.get_dev_cards().size() == 1);
    }

    SUBCASE("try to buy dev card without any resources") {
        Player player1(PlayerColor::RED);
        Catan catan(player1, player1, player1);

        CHECK_THROWS(player1.buy_dev_card(catan));
    }
}