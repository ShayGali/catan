#pragma once

#include "Card.hpp"

class YearOfPlentyCard : public Card {
    CardType get_type() override;
    std::string get_description() override;
};
