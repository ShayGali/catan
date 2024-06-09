#pragma once

#include "PromotionCard.hpp"

class RoadBuildCard : public PromotionCard {
    std::string get_description() const override;
    std::string emoji() const override;
    CardType type() const override;
    Card* clone() const override;
    /**
     * @brief use the road building card - will ask the user to where to build the roads
     */
    void use(Catan& game, Player& player) override;
};
