#include "KngihtCard.hpp"


CardType KnightCard::get_type() {
    return CardType::KNIGHT;
}

std::string KnightCard::get_description() {
    return "If you have 3 🗡️ you will get 2 victory points";
}

std::string KnightCard::emoji() {
    return "🗡️";
}
