#pragma once
#include "Card.hpp"

class PromotionCard : public Card {
   public:
    virtual void use(Catan& catan, Player& player) = 0;
};