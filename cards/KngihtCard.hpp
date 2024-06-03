#pragma once

#include "Card.hpp"

class KnightCard : public Card {
    CardType get_type() override;
    std::string get_description() override;
};