#pragma once

#include "PromotionCard.hpp"

class YearOfPlentyCard : public PromotionCard {
    std::string get_description() override;
    std::string emoji() override;
    CardType type() override;

    void use(Catan& game, Player& player) override;
};
