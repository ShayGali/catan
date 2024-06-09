#pragma once

#include "PromotionCard.hpp"

class MonopolyCard : public PromotionCard {
    std::string get_description() const override;
    std::string emoji() const override;
    CardType type() const override;
    Card* clone() const override;

    /**
     * @brief use the monopoly card - will ask the player to choose a resource and take all of the chosen resource from all the players
     */
    void use(Catan& game, Player& player) override;
};
