#pragma once

#include "PromotionCard.hpp"

class YearOfPlentyCard : public PromotionCard {
    CardType get_type() override;
    std::string get_description() override;
    std::string emoji() override;
    void use(Catan& game, Player& player) override;
};
