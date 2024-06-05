#pragma once

#include "Card.hpp"

class RoadBuildCard : public Card {
    CardType get_type() override;
    std::string get_description() override;
    std::string emoji() override;
    void use(Catan& game, Player& player) override;
};
