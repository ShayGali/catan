#pragma once
#include <string>
enum class CardType {
    KNIGHT,
    VICTORY_POINT,
    ROAD_BUILDING,
    MONOPOLY,
    YEAR_OF_PLENTY
};

// Forward declaration
class Catan;
class Player;

class Card {
   public:
    virtual CardType get_type() = 0;
    virtual std::string get_description() = 0;
    // virtual void play(Catan& game, Player& player) = 0;
};