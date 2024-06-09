#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <iostream>

#include "../Catan.hpp"
#include "../cards/KngihtCard.hpp"
#include "../cards/MonopolyCard.hpp"
#include "../cards/RoadBuildCard.hpp"
#include "../cards/VictoryPointCard.hpp"
#include "../cards/YearOfPlentyCard.hpp"
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

TEST_CASE("7 roll") {
    Player player1(PlayerColor::RED);
    Player player2(PlayerColor::BLUE);
    Player player3(PlayerColor::YELLOW);
    Catan catan(player1, player2, player3);
    // change the seed such thar the 7 will be rolled
    srand(1);

    SUBCASE("no one has more than 7 resources") {
        player1.add_resource(resource::WHEAT, 2);
        player1.add_resource(resource::STONE, 3);

        catan.roll_dice();
    }

    SUBCASE("one player has more than 7 resources") {
        player1.add_resource(resource::WOOD, 3);
        player1.add_resource(resource::CLAY, 2);
        player1.add_resource(resource::SHEEP, 1);
        player1.add_resource(resource::WHEAT, 1);
        player1.add_resource(resource::STONE, 1);

        // change cin to input buffer
        istringstream input("1 1 1 0 1\n");
        streambuf* prevcinbuf = cin.rdbuf(input.rdbuf());
        catan.roll_dice();
        // change cin back to standard input
        cin.rdbuf(prevcinbuf);

        // check that the resources are taken from the player
        CHECK((player1.get_resource_count(resource::WOOD) == 2 && player1.get_resource_count(resource::CLAY) == 1 && player1.get_resource_count(resource::SHEEP) == 0 && player1.get_resource_count(resource::WHEAT) == 1 && player1.get_resource_count(resource::STONE) == 0));
    }

    SUBCASE("two players have more than 7 resources") {
        player1.add_resource(resource::WOOD, 3);
        player1.add_resource(resource::CLAY, 2);
        player1.add_resource(resource::SHEEP, 1);
        player1.add_resource(resource::WHEAT, 1);
        player1.add_resource(resource::STONE, 1);

        player2.add_resource(resource::WOOD, 3);
        player2.add_resource(resource::CLAY, 2);
        player2.add_resource(resource::SHEEP, 1);
        player2.add_resource(resource::WHEAT, 1);
        player2.add_resource(resource::STONE, 1);

        // change cin to input buffer
        istringstream input("1 1 1 0 1\n1 1 1 0 1\n");
        streambuf* prevcinbuf = cin.rdbuf(input.rdbuf());
        catan.roll_dice();
        // change cin back to standard input
        cin.rdbuf(prevcinbuf);

        // check that the resources are taken from the player
        CHECK((player1.get_resource_count(resource::WOOD) == 2 && player1.get_resource_count(resource::CLAY) == 1 && player1.get_resource_count(resource::SHEEP) == 0 && player1.get_resource_count(resource::WHEAT) == 1 && player1.get_resource_count(resource::STONE) == 0));
        CHECK((player2.get_resource_count(resource::WOOD) == 2 && player2.get_resource_count(resource::CLAY) == 1 && player2.get_resource_count(resource::SHEEP) == 0 && player2.get_resource_count(resource::WHEAT) == 1 && player2.get_resource_count(resource::STONE) == 0));
    }

    SUBCASE("player try to return wrong number of resources") {
        player1.add_resource(resource::WOOD, 3);
        player1.add_resource(resource::CLAY, 2);
        player1.add_resource(resource::SHEEP, 1);
        player1.add_resource(resource::WHEAT, 1);
        player1.add_resource(resource::STONE, 1);

        // change cin to input buffer
        // need to return 4 resources
        // try to return 5,0,8
        // and then return 4
        istringstream input("2 1 1 0 1\n0 0 0 0 0\n3 2 1 1 1\n0 1 1 1 1\n");
        streambuf* prevcinbuf = cin.rdbuf(input.rdbuf());
        catan.roll_dice();
        // change cin back to standard input
        cin.rdbuf(prevcinbuf);

        // check that the resources are taken from the player
        player1.display_resources();
        CHECK((player1.get_resource_count(resource::WOOD) == 3 && player1.get_resource_count(resource::CLAY) == 1 && player1.get_resource_count(resource::SHEEP) == 0 && player1.get_resource_count(resource::WHEAT) == 0 && player1.get_resource_count(resource::STONE) == 0));
    }

    SUBCASE("try to return more than what he has") {
        player1.add_resource(resource::WOOD, 3);
        player1.add_resource(resource::CLAY, 2);
        player1.add_resource(resource::SHEEP, 1);
        player1.add_resource(resource::WHEAT, 1);
        player1.add_resource(resource::STONE, 1);

        // change cin to input buffer
        istringstream input("4 3 1 0 0 0\n");
        streambuf* prevcinbuf = cin.rdbuf(input.rdbuf());
        catan.roll_dice();
        // change cin back to standard input
        cin.rdbuf(prevcinbuf);
        CHECK((player1.get_resource_count(resource::WOOD) == 0 && player1.get_resource_count(resource::CLAY) == 1 && player1.get_resource_count(resource::SHEEP) == 1 && player1.get_resource_count(resource::WHEAT) == 1 && player1.get_resource_count(resource::STONE) == 1));
    }
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

TEST_CASE("trade tests") {
    Player player1(PlayerColor::RED);
    Player player2(PlayerColor::BLUE);
    Player player3(PlayerColor::YELLOW);
    Catan catan(player1, player2, player3);

    SUBCASE("simple trade - RED will trade 1 WOOD & 1 victory point card for 1 STONE & 1 knight card") {
        // the card will be free in the player destructor
        Card* k_card = new KnightCard();
        Card* vp_card = new VictoryPointCard();

        player1.add_resource(resource::WOOD, 1);
        player1.add_resource(resource::CLAY, 1);

        player1.add_dev_card(vp_card);
        player1.add_victory_points(1);

        player2.add_resource(resource::STONE, 1);
        player2.add_dev_card(k_card);
        player2.add_knight();

        vector<pair<resource, int>> offer_res = {{resource::WOOD, 1}};
        vector<Card*> offer_dev = {vp_card};
        vector<pair<resource, int>> request = {{resource::STONE, 1}};
        vector<pair<CardType, int>> request_dev = {{CardType::KNIGHT, 1}};

        // make sure that all the before are good

        // player1
        CHECK(player1.get_resource_count(resource::WOOD) == 1);
        CHECK(player1.get_resource_count(resource::CLAY) == 1);
        CHECK(player1.get_dev_cards().size() == 1);
        CHECK(player1.get_dev_cards()[0] == vp_card);
        CHECK(player1.get_victory_points() == 1);
        CHECK(player1.get_knights() == 0);

        // player2
        CHECK(player2.get_resource_count(resource::STONE) == 1);
        CHECK(player2.get_dev_cards().size() == 1);
        CHECK(player2.get_dev_cards()[0] == k_card);
        CHECK(player2.get_knights() == 1);
        CHECK(player2.get_victory_points() == 0);

        // redirect cin to input buffer
        istringstream input("y\n");
        streambuf* prevcinbuf = cin.rdbuf(input.rdbuf());

        catan.make_trade_offer(player1, offer_res, offer_dev, request, request_dev);

        // redirect cin back to standard input
        cin.rdbuf(prevcinbuf);

        // check that the trade is done

        // player1
        CHECK(player1.get_resource_count(resource::WOOD) == 0);
        CHECK(player1.get_resource_count(resource::CLAY) == 1);
        CHECK(player1.get_resource_count(resource::STONE) == 1);
        CHECK(player1.get_dev_cards().size() == 1);
        CHECK(player1.get_dev_cards()[0] == k_card);
        CHECK(player1.get_victory_points() == 0);
        CHECK(player1.get_knights() == 1);

        // player2
        CHECK(player2.get_resource_count(resource::STONE) == 0);
        CHECK(player2.get_resource_count(resource::WOOD) == 1);
        CHECK(player2.get_dev_cards().size() == 1);
        CHECK(player2.get_dev_cards()[0] == vp_card);
        CHECK(player2.get_knights() == 0);
        CHECK(player2.get_victory_points() == 1);
    }

    SUBCASE("no one will accept the trade") {
        player1.add_resource(resource::WOOD, 1);

        vector<pair<resource, int>> offer_res = {{resource::WOOD, 1}};
        vector<Card*> offer_dev = {};
        vector<pair<resource, int>> request = {{resource::STONE, 1}};
        vector<pair<CardType, int>> request_dev = {};

        // redirect cin to input buffer
        istringstream input("n n\n");
        streambuf* prevcinbuf = cin.rdbuf(input.rdbuf());

        catan.make_trade_offer(player1, offer_res, offer_dev, request, request_dev);

        // redirect cin back to standard input
        cin.rdbuf(prevcinbuf);

        // check that the trade is not done
        CHECK(player1.get_resource_count(resource::WOOD) == 1);
    }

    SUBCASE("player dont have enough resources to make offer") {
        Card* k_card = new KnightCard();  // the card will be free in the end of the test
        player1.add_resource(resource::WOOD, 1);

        vector<pair<resource, int>> offer_res = {{resource::WOOD, 2}};
        vector<Card*> offer_dev = {};
        vector<pair<resource, int>> request = {{resource::STONE, 1}};
        vector<pair<CardType, int>> request_dev = {};

        CHECK_THROWS(catan.make_trade_offer(player1, offer_res, offer_dev, request, request_dev));

        offer_res = {};
        offer_dev = {k_card};
        CHECK_THROWS(catan.make_trade_offer(player1, offer_res, offer_dev, request, request_dev));

        delete k_card;
    }

    SUBCASE("player try to accept the trade with out enough resources") {
        player1.add_resource(resource::WOOD, 1);
        player1.add_resource(resource::STONE, 2);
        player2.add_resource(resource::STONE, 1);

        vector<pair<resource, int>> offer_res = {{resource::WOOD, 1}};
        vector<Card*> offer_dev = {};
        vector<pair<resource, int>> request = {{resource::STONE, 2}};
        vector<pair<CardType, int>> request_dev = {};

        // redirect cin to input buffer
        istringstream input("y y\n");
        streambuf* prevcinbuf = cin.rdbuf(input.rdbuf());

        catan.make_trade_offer(player1, offer_res, offer_dev, request, request_dev);

        // redirect cin back to standard input
        cin.rdbuf(prevcinbuf);

        // check that the trade is not done
        CHECK(player1.get_resource_count(resource::WOOD) == 1);
        CHECK(player1.get_resource_count(resource::STONE) == 2);
        CHECK(player2.get_resource_count(resource::STONE) == 1);
    }

    SUBCASE("player try to accept the trade with out enough dev cards") {
        player1.add_resource(resource::WOOD, 1);
        player2.add_resource(resource::STONE, 1);

        vector<pair<resource, int>> offer_res = {{resource::WOOD, 1}};
        vector<Card*> offer_dev = {};
        vector<pair<resource, int>> request = {{resource::STONE, 1}};
        vector<pair<CardType, int>> request_dev = {{CardType::KNIGHT, 1}};

        // redirect cin to input buffer
        istringstream input("y y\n");
        streambuf* prevcinbuf = cin.rdbuf(input.rdbuf());

        catan.make_trade_offer(player1, offer_res, offer_dev, request, request_dev);

        // redirect cin back to standard input
        cin.rdbuf(prevcinbuf);

        // check that the trade is not done
        CHECK(player1.get_resource_count(resource::WOOD) == 1);
        CHECK(player2.get_resource_count(resource::STONE) == 1);
        CHECK(player1.get_dev_cards().size() == 0);
        CHECK(player1.get_knights() == 0);
        CHECK(player2.get_dev_cards().size() == 0);
    }
}

TEST_CASE("use promotion card") {
    Player player1(PlayerColor::RED), player2(PlayerColor::BLUE), player3(PlayerColor::YELLOW);
    Catan catan(player1, player2, player3);

    SUBCASE("Monopoly") {
        MonopolyCard* monopoly = new MonopolyCard();  // the card will be free in the player destructor

        // give wood to all the players
        player1.add_resource(resource::WOOD, 1);
        player2.add_resource(resource::WOOD, 2);
        player3.add_resource(resource::WOOD, 1);

        // give player1 a monopoly card
        player1.add_dev_card(monopoly);

        // change cin to input buffer
        istringstream input("1\n");
        streambuf* prevcinbuf = cin.rdbuf(input.rdbuf());

        player1.use_dev_card(catan, monopoly);
        // change cin back to standard input
        cin.rdbuf(prevcinbuf);

        // check that the resources are taken from the players
        CHECK(player1.get_resource_count(resource::WOOD) == 4);
        CHECK(player2.get_resource_count(resource::WOOD) == 0);
        CHECK(player3.get_resource_count(resource::WOOD) == 0);
    }

    SUBCASE("Year of plenty") {
        YearOfPlentyCard* year_of_plenty = new YearOfPlentyCard();  // the card will be free in the player destructor

        // give player1 a year of plenty card
        player1.add_dev_card(year_of_plenty);

        // change cin to input buffer
        istringstream input("1 2\n");
        streambuf* prevcinbuf = cin.rdbuf(input.rdbuf());

        player1.use_dev_card(catan, year_of_plenty);
        // change cin back to standard input
        cin.rdbuf(prevcinbuf);

        // check that the resources are taken from the players
        CHECK(player1.get_resource_count(resource::WOOD) == 1);
        CHECK(player1.get_resource_count(resource::CLAY) == 1);
    }
    SUBCASE("Road building") {
        // place a settlement and a road
        catan.place_settlement(8, player1, true);

        // give player1 a road building card
        RoadBuildCard* road_building = new RoadBuildCard();  // the card will be free in the player destructor

        player1.add_dev_card(road_building);

        // change cin to input buffer
        istringstream input("12 13\n");
        streambuf* prevcinbuf = cin.rdbuf(input.rdbuf());

        player1.use_dev_card(catan, road_building);
        // change cin back to standard input
        cin.rdbuf(prevcinbuf);

        // make sure that the roads are placed
        CHECK((catan.get_edges()[12].get_owner() == &player1 && catan.get_edges()[13].get_owner() == &player1));
    }
}
