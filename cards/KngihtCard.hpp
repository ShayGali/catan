#pragma once

#include "Card.hpp"

class KnightCard : public Card {
    std::string get_description() override;
    std::string emoji() override;
};