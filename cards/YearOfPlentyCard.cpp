#include "YearOfPlentyCard.hpp"

CardType YearOfPlentyCard::get_type() {
    return CardType::YEAR_OF_PLENTY;
}

std::string YearOfPlentyCard::get_description() {
    return "Year of Plenty: Gain any two resources from the bank.";
}

std::string YearOfPlentyCard::emoji() {
    return "🌟";
}