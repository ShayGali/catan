#pragma once

#include "PromotionCard.hpp"

class RoadBuildCard : public PromotionCard {
    std::string get_description() const override;
    std::string emoji() const override;
    CardType type() const override;
    Card* clone() const override;
    void use(Catan& game, Player& player) override;
};
