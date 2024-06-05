#pragma once

#include "Card.hpp"

class VictoryPointCard : public Card {
    std::string get_description() override;
    std::string emoji() override;
};
