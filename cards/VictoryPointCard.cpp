#include "VictoryPointCard.hpp"

CardType VictoryPointCard::get_type() {
    return CardType::VICTORY_POINT;
}

std::string VictoryPointCard::get_description() {
    return "This card gives you 1 victory point";
}

std::string VictoryPointCard::emoji() {
    return "🏆";
}

void VictoryPointCard::use(Catan& game, Player& player) {
    throw std::runtime_error("VictoryPointCard::use() should not be called");
}