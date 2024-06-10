// @author: shay.gali@msmail.ariel.ac.il
#include "RoadBuildCard.hpp"

#include "../player/Player.hpp"

std::string RoadBuildCard::get_description() const {
    return "You may immediately place 2 free roads";
}

std::string RoadBuildCard::emoji() const {
    return "🛣️";
}

CardType RoadBuildCard::type() const {
    return CardType::ROAD_BUILDING;
}

Card* RoadBuildCard::clone() const {
    return new RoadBuildCard(*this);
}

void RoadBuildCard::use(Catan& game, Player& player) {
    std::cout << "You may immediately place 2 free roads.\n";
    player.place_road(game, true);
    player.place_road(game, true);
}
