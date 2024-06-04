#include "MonopolyCard.hpp"

CardType MonopolyCard::get_type() {
    return CardType::MONOPOLY;
}

std::string MonopolyCard::get_description() {
    return "Monopoly: When you play this card, you choose a resource type. All other players must give you all of their resources of that type.";
}

std::string MonopolyCard::emoji() {
    return "🏦";
}