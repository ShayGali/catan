#pragma once

#include "Card.hpp"

class KnightCard : public Card {
    std::string get_description() const override;
    std::string emoji() const override;
    CardType type() const override;
    Card* clone() const override;
};