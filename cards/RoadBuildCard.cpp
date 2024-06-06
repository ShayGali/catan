#include "RoadBuildCard.hpp"

#include "../player/Player.hpp"

std::string RoadBuildCard::get_description() {
    return "You may immediately place 2 free roads";
}

std::string RoadBuildCard::emoji() {
    return "🛣️";
}

CardType RoadBuildCard::type() {
    return CardType::ROAD_BUILDING;
}

void RoadBuildCard::use(Catan& game, Player& player) {
    std::cout << "You may immediately place 2 free roads.\n";
    player.place_road(game, true);
    player.place_road(game, true);
}
