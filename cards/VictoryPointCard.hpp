#pragma once

#include "Card.hpp"

class VictoryPointCard : public Card {
    std::string get_description() const override;
    std::string emoji() const override;
    CardType type() const override;
    Card* clone() const override;
};
