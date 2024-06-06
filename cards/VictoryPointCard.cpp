#include "VictoryPointCard.hpp"


std::string VictoryPointCard::get_description() {
    return "This card gives you 1 victory point";
}

std::string VictoryPointCard::emoji() {
    return "🏆";
}

CardType VictoryPointCard::type(){
    return CardType::VICTORY_POINT;
}

Card* VictoryPointCard::clone() {
    return new VictoryPointCard(*this);
}