#include "KngihtCard.hpp"

std::string KnightCard::get_description() {
    return "If you have 3 🗡️ you will get 2 victory points";
}

std::string KnightCard::emoji() {
    return "🗡️";
}

CardType KnightCard::type() {
    return CardType::KNIGHT;
}
