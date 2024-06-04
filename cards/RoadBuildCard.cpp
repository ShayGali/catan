#include "RoadBuildCard.hpp"

CardType RoadBuildCard::get_type() {
    return CardType::ROAD_BUILDING;
}

std::string RoadBuildCard::get_description() {
    return "You may immediately place 2 free roads";
}

std::string RoadBuildCard::emoji() {
    return "🛣️";
}