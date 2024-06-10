// @author: shay.gali@msmail.ariel.ac.il
#pragma once

#include <string>
#include <stdexcept>

// https://stackoverflow.com/questions/21295935/can-a-c-enum-class-have-methods

/**
 * @brief Enum class to represent the resources in the game.
 */
class resource {
   public:
    enum Value {
        WOOD,
        CLAY,
        SHEEP,
        WHEAT,
        STONE,
        DESERT,  // IMPORTANT: This is not a resource, but it is used to represent the desert tile (need to be the last element in the enum)
        NONE     // This is used to represent the absence of a resource
    };

    resource() = default;
    resource(Value value) : value(value) {}

    std::string get_emoji() const {
        switch (this->value) {
            case Value::WOOD:
                return "🌲";
            case Value::CLAY:
                return "🧱";
            case Value::SHEEP:
                return "🐑";
            case Value::WHEAT:
                return "🌾";
            case Value::STONE:
                return "⛰️ ";
            case resource::DESERT:
                return "🏜️";
            case resource::NONE:
                return "";
        }
        return "";
    }

    int get_int() const {
        return this->value;
    }

    static resource from_int(int value) {
        switch (value) {
            case 0:
                return resource(resource::Value::WOOD);
            case 1:
                return resource(resource::Value::CLAY);
            case 2:
                return resource(resource::Value::SHEEP);
            case 3:
                return resource(resource::Value::WHEAT);
            case 4:
                return resource(resource::Value::STONE);
            case 5:
                return resource(resource::Value::DESERT);
            case 6:
                return resource(resource::Value::NONE);
            default:
                throw std::invalid_argument("Invalid resource value");
        }
    }

    // Allow switch and comparisons.
    constexpr operator Value() const { return value; }

    // Prevent usage: if(fruit)
    explicit operator bool() const = delete;

   private:
    Value value;
};
