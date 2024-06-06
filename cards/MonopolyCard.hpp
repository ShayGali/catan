#pragma once

#include "PromotionCard.hpp"

class MonopolyCard : public PromotionCard {
    std::string get_description() override;
    std::string emoji() override;
    CardType type() override;
    Card* clone() override;
    void use(Catan& game, Player& player) override;
};
