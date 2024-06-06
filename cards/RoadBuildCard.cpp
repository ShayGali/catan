#include "RoadBuildCard.hpp"

std::string RoadBuildCard::get_description() {
    return "You may immediately place 2 free roads";
}

std::string RoadBuildCard::emoji() {
    return "🛣️";
}

CardType RoadBuildCard::type(){
    return CardType::ROAD_BUILDING;
}

void RoadBuildCard::use(Catan& game, Player& player) {
    throw std::runtime_error("Not implemented");
}
