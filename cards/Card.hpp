#pragma once
#include <stdexcept>
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
    virtual std::string emoji() = 0;
    virtual ~Card() = default;

    static CardType from_int(int type) {
        switch (type) {
            case 0:
                return CardType::KNIGHT;
            case 1:
                return CardType::VICTORY_POINT;
            case 2:
                return CardType::ROAD_BUILDING;
            case 3:
                return CardType::MONOPOLY;
            case 4:
                return CardType::YEAR_OF_PLENTY;
            default:
                throw std::invalid_argument("Invalid card type");
        }
    }
};