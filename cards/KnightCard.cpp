// @author: shay.gali@msmail.ariel.ac.il
#include "KnightCard.hpp"

std::string KnightCard::get_description() const {
    return "If you have 3 ⚔️  you will get 2 victory points";
}

std::string KnightCard::emoji() const {
    return "⚔️";
}

CardType KnightCard::type() const {
    return CardType::KNIGHT;
}

Card* KnightCard::clone() const {
    return new KnightCard(*this);
}
