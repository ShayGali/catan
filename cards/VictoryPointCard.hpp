#pragma once

#include "Card.hpp"

class VictoryPointCard : public Card {
    CardType get_type() override;
    std::string get_description() override;
    std::string emoji() override;

};
