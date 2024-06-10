// @author: shay.gali@msmail.ariel.ac.il
#include "VictoryPointCard.hpp"

std::string VictoryPointCard::get_description() const {
    return "This card gives you 1 victory point";
}

std::string VictoryPointCard::emoji() const {
    return "🏆";
}

CardType VictoryPointCard::type() const {
    return CardType::VICTORY_POINT;
}

Card* VictoryPointCard::clone() const {
    return new VictoryPointCard(*this);
}