#include "YearOfPlentyCard.hpp"


std::string YearOfPlentyCard::get_description() {
    return "Year of Plenty: Gain any two resources from the bank.";
}

std::string YearOfPlentyCard::emoji() {
    return "🌟";
}

CardType YearOfPlentyCard::type(){
    return CardType::YEAR_OF_PLENTY;
}

void YearOfPlentyCard::use(Catan& game, Player& player) {
    throw std::runtime_error("Not implemented");
}