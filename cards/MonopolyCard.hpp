#pragma once

#include "PromotionCard.hpp"

class MonopolyCard : public PromotionCard {
    std::string get_description() override;
    std::string emoji() override;
    void use(Catan& game, Player& player) override;
};
