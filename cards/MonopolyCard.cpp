#include "MonopolyCard.hpp"


std::string MonopolyCard::get_description() {
    return "Monopoly: When you play this card, you choose a resource type. All other players must give you all of their resources of that type.";
}

std::string MonopolyCard::emoji() {
    return "🏦";
}

CardType MonopolyCard::type(){
    return CardType::MONOPOLY;
}

void MonopolyCard::use(Catan& game, Player& player) {
    throw std::runtime_error("Not implemented");
}