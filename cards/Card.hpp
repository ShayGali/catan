#pragma once
#include <iostream>
#include <stdexcept>
#include <string>

#include "../game_piece/resource.hpp"

enum class CardType {
    KNIGHT,
    VICTORY_POINT,
    ROAD_BUILDING,
    MONOPOLY,
    YEAR_OF_PLENTY
};


class Card {
   public:
    virtual std::string get_description() = 0;
    virtual std::string emoji() = 0;
    virtual CardType type() = 0;
    virtual Card* clone() = 0;
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

    static std::string emoji_from_type(CardType type) {
        switch (type) {
            case CardType::KNIGHT:
                return "⚔️";
            case CardType::VICTORY_POINT:
                return "🏆";
            case CardType::ROAD_BUILDING:
                return "🛣️";
            case CardType::MONOPOLY:
                return "🏦";
            case CardType::YEAR_OF_PLENTY:
                return "🌟";
            default:
                throw std::invalid_argument("Invalid card type");
        }
    }
};