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
    /**
     * @brief Get the description of the card
     */
    virtual std::string get_description() const = 0;

    /**
     * @brief Get the emoji of the card
     */
    virtual std::string emoji() const = 0;
    /**
     * @brief Get the type of the card
     */
    virtual CardType type() const = 0;

    /**
     * @brief return a deep copy of the card. the copy is allocated on the heap
     */
    virtual Card* clone() const = 0;
    virtual ~Card() = default;

    /**
     * @brief get card type from int.
     * @param type - the type of the card as an int
     * Knight = 0, VictoryPoint = 1, RoadBuilding = 2, Monopoly = 3, YearOfPlenty = 4
     * @throws invalid_argument if the type is invalid
     */
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

    /**
     * @brief get emoji from card type
     * @param type - the type of the card
     * @throws invalid_argument if the type is invalid
     */
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