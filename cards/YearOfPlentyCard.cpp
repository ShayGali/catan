#include "YearOfPlentyCard.hpp"


std::string YearOfPlentyCard::get_description() {
    return "Year of Plenty: Gain any two resources from the bank.";
}

std::string YearOfPlentyCard::emoji() {
    return "🌟";
}

void YearOfPlentyCard::use(Catan& game, Player& player) {
    throw std::runtime_error("Not implemented");
}