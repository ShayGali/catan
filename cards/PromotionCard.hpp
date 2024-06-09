#pragma once
#include "Card.hpp"

// Forward declaration
class Catan;
class Player;

class PromotionCard : public Card {
   public:

   /**
    * @brief use the promotion card
    */
    virtual void use(Catan& catan, Player& player) = 0;
};