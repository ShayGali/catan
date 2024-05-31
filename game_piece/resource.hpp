#pragma once

#include <string>
// https://stackoverflow.com/questions/21295935/can-a-c-enum-class-have-methods
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

    std::string get_emoji() {
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

    // Allow switch and comparisons.
    constexpr operator Value() const { return value; }

    // Prevent usage: if(fruit)
    explicit operator bool() const = delete;

   private:
    Value value;
};
