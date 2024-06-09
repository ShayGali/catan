#pragma once

#include "PromotionCard.hpp"

class YearOfPlentyCard : public PromotionCard {
    std::string get_description() const override;
    std::string emoji() const override;
    CardType type() const override;
    Card* clone() const override;
    /**
     * @brief use the year of plenty card - will ask the user to choose two resources and give them to the player
     */
    void use(Catan& game, Player& player) override;
};
